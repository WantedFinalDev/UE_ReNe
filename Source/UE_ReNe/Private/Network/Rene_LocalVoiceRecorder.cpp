// Rene_LocalVoiceRecorder.cpp

#include "Network/Rene_LocalVoiceRecorder.h"
#include "Modules/ModuleManager.h"
#include "VoiceModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Controller.h"


URene_LocalVoiceRecorder::URene_LocalVoiceRecorder()
{
    PrimaryComponentTick.bCanEverTick = false;
    bIsRecording = false;
}

void URene_LocalVoiceRecorder::BeginPlay()
{
    Super::BeginPlay();
    InitializeVoiceCapture();
}

void URene_LocalVoiceRecorder::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Clean up resources
    if (VoiceCapture.IsValid())
    {
        if (bIsRecording)
        {
            GetWorld()->GetTimerManager().ClearTimer(VoiceCaptureTimerHandle);
            VoiceCapture->Stop();
        }
        VoiceCapture.Reset();
    }
    Super::EndPlay(EndPlayReason);
}

void URene_LocalVoiceRecorder::InitializeVoiceCapture()
{
    // The FVoiceModule is required for voice capture.
    FVoiceModule& VoiceModule = FModuleManager::LoadModuleChecked<FVoiceModule>("Voice");
    VoiceCapture = VoiceModule.CreateVoiceCapture(FString());

    if (VoiceCapture.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("VoiceCapture device initialized successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize VoiceCapture device."));
    }
}

void URene_LocalVoiceRecorder::StartRecording()
{
    if (!VoiceCapture.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("StartRecording failed: VoiceCapture is not valid."));
        return;
    }

    if (bIsRecording)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartRecording called while already recording."));
        return;
    }

    bIsRecording = true;
    
    // Clear any previously recorded data.
    {
        FScopeLock Lock(&VoiceBufferCriticalSection);
        RecordedVoiceData.Empty();
    }

    // Start capturing audio from the default microphone.
    if (VoiceCapture->Start())
    {
        UE_LOG(LogTemp, Log, TEXT("Local voice recording started."));

        // Poll for voice data on a timer. A 20ms interval is a good starting point.
        // This is preferred over Tick to have explicit control over the polling frequency.
        GetWorld()->GetTimerManager().SetTimer(
            VoiceCaptureTimerHandle,
            this,
            &URene_LocalVoiceRecorder::CaptureVoiceData,
            0.02f,
            true);
    }
    else
    {
        bIsRecording = false;
        UE_LOG(LogTemp, Error, TEXT("Failed to start voice capture."));
    }
}

void URene_LocalVoiceRecorder::CaptureVoiceData()
{
    if (!bIsRecording || !VoiceCapture.IsValid())
    {
        return;
    }

    uint32 BytesAvailable = 0;
    EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(BytesAvailable);

    if (CaptureState == EVoiceCaptureState::Ok && BytesAvailable > 0)
    {
        TArray<uint8> TempBuffer;
        TempBuffer.SetNumUninitialized(BytesAvailable);

        uint32 BytesRead = 0;
        VoiceCapture->GetVoiceData(TempBuffer.GetData(), BytesAvailable, BytesRead);

        if (BytesRead > 0)
        {
            FScopeLock Lock(&VoiceBufferCriticalSection);
            // Append the new data to our main buffer
            RecordedVoiceData.Append(TempBuffer.GetData(), BytesRead);
        }
    }
}

void URene_LocalVoiceRecorder::StopAndUploadRecording()
{
    if (!bIsRecording)
    {
        return;
    }

    bIsRecording = false;
    
    // Stop the polling timer.
    GetWorld()->GetTimerManager().ClearTimer(VoiceCaptureTimerHandle);

	// Perform one final capture to get any data buffered since the last timer tick.
	CaptureVoiceData();

    if (VoiceCapture.IsValid())
    {
        VoiceCapture->Stop();
        UE_LOG(LogTemp, Log, TEXT("Local voice recording stopped."));
    }

    TArray<uint8> VoiceDataToUpload;
    {
        FScopeLock Lock(&VoiceBufferCriticalSection);
        if (RecordedVoiceData.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("No voice data was recorded. Skipping upload."));
            return;
        }
        // Make a copy for the HTTP request, as it will be processed on another thread.
        VoiceDataToUpload = RecordedVoiceData;
        RecordedVoiceData.Empty();
    }

    UE_LOG(LogTemp, Log, TEXT("Uploading %d bytes of voice data."), VoiceDataToUpload.Num());

    // Get Player Name from the owning PlayerController
    FString PlayerName = TEXT("UnknownPlayer");
    
    // 0번 로컬 플레이어의 컨트롤러를 가져옵니다. (로컬 클라이언트 자신을 의미)
    if (APlayerController* LocalPC = GetWorld()->GetFirstPlayerController())
    {
        if (ULocalPlayer* LocalPlayer = LocalPC->GetLocalPlayer())
        {
            PlayerName = LocalPlayer->GetNickname();
            UE_LOG(LogTemp, Log, TEXT("Player Name Retrieved: %s (Successful)"), *PlayerName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Could not find Local Player from PlayerController."));
        }
        /*
        // 컨트롤러에서 PlayerState를 가져옵니다. PlayerState는 모든 클라이언트에 복제됩니다.
        if (APlayerState* PlayerState = LocalPC->GetPlayerState<APlayerState>())
        {
            PlayerName = PlayerState->GetPlayerName();
            UE_LOG(LogTemp, Log, TEXT("Player Name Retrieved: %s (Successful)"), *PlayerName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Could not find Local PlayerState."));
        }
        */
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find Local PlayerController (Index 0)."));
    }
    SendHttpRequest(VoiceDataToUpload, PlayerName);
}

void URene_LocalVoiceRecorder::SendHttpRequest(const TArray<uint8>& VoiceData, const FString& PlayerName)
{
    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &URene_LocalVoiceRecorder::OnUploadComplete);
    Request->SetURL(HttpUploadURL);
    Request->SetVerb(TEXT("POST"));

    // Determine speaker_role based on authority
    FString SpeakerRole;
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        SpeakerRole = TEXT("company");
    }
    else
    {
        SpeakerRole = TEXT("seeker");
    }

    // --- Manually create the multipart/form-data payload ---
    FString Boundary = FString::Printf(TEXT("rene-voice-boundary-%s"), *FGuid::NewGuid().ToString());

    Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *Boundary));

    TArray<uint8> RequestPayload;
    
    // 헬퍼 람다 함수: 문자열을 UTF8로 변환하여 안전하게 페이로드에 추가
    auto AddStringField = [&](const FString& InString)
    {
        FTCHARToUTF8 Convert(*InString);
        RequestPayload.Append((uint8*)Convert.Get(), Convert.Length());
    };

    const FString BoundaryLine = TEXT("--") + Boundary + TEXT("\r\n");
    const FString BoundaryEnd = TEXT("\r\n--") + Boundary + TEXT("--\r\n");

    // 1. Append speaker_role part
    AddStringField(BoundaryLine);
    
    FString SpeakerRoleHeader = FString::Printf(TEXT("Content-Disposition: form-data; name=\"speaker_role\"\r\n\r\n"));
    AddStringField(SpeakerRoleHeader);
    AddStringField(SpeakerRole);
    AddStringField(TEXT("\r\n")); // [수정됨] 여기서 *("\r\n")을 사용하여 크래시가 났었음

    // 2. Append file part (VoiceData)
    // Generate a unique filename using PlayerName and a timestamp
    FString Timestamp = FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S"));
    FString UniqueFileName = FString::Printf(TEXT("%s_%s_%s.pcm"), *SpeakerRole, *PlayerName, *Timestamp);

    AddStringField(BoundaryLine);

    FString FileHeader = FString::Printf(TEXT("Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n"), *UniqueFileName);
    FileHeader += TEXT("Content-Type: application/octet-stream\r\n\r\n");
    
    AddStringField(FileHeader);

    // Append the actual voice data
    RequestPayload.Append(VoiceData);

    // Append the final boundary
    AddStringField(BoundaryEnd);

    Request->SetContent(RequestPayload);
    Request->ProcessRequest();
}

void URene_LocalVoiceRecorder::OnUploadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString ResponseBody = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Voice data uploaded successfully. Server response: %s"), *ResponseBody);
        OnUploadSuccess.Broadcast(ResponseBody);
    }
    else
    {
        FString ErrorMessage = TEXT("Unknown error");
        if (Response.IsValid())
        {
            ErrorMessage = FString::Printf(TEXT("HTTP %d: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
        }
        else
        {
            ErrorMessage = TEXT("HTTP request failed or no response received.");
        }
        UE_LOG(LogTemp, Error, TEXT("Voice data upload failed. Reason: %s"), *ErrorMessage);
        OnUploadFailure.Broadcast(ErrorMessage);
    }
}
