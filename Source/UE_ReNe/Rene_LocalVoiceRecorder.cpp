// Rene_LocalVoiceRecorder.cpp

#include "Rene_LocalVoiceRecorder.h"
#include "Modules/ModuleManager.h"
#include "VoiceModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

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
    SendHttpRequest(VoiceDataToUpload);
}

void URene_LocalVoiceRecorder::SendHttpRequest(const TArray<uint8>& VoiceData)
{
    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &URene_LocalVoiceRecorder::OnUploadComplete);
    Request->SetURL(HttpUploadURL);
    Request->SetVerb(TEXT("POST"));

    // --- Manually create the multipart/form-data payload ---
    FString Boundary = FString::Printf(TEXT("rene-voice-boundary-%s"), *FGuid::NewGuid().ToString());

    Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *Boundary));

    TArray<uint8> RequestPayload;
    const FString BoundaryPrefix = TEXT("--") + Boundary + TEXT("\r\n");
    const FString BoundarySuffix = TEXT("\r\n--") + Boundary + TEXT("--\r\n");

    // Append boundary and headers for the binary file part
    RequestPayload.Append((uint8*)TCHAR_TO_UTF8(*BoundaryPrefix), BoundaryPrefix.Len());
    FString FileHeader = TEXT("Content-Disposition: form-data; name=\"voice\"; filename=\"voice.pcm\"\r\n");
    FileHeader += TEXT("Content-Type: application/octet-stream\r\n\r\n");
    RequestPayload.Append((uint8*)TCHAR_TO_UTF8(*FileHeader), FileHeader.Len());

    // Append the actual voice data
    RequestPayload.Append(VoiceData);

    // Append the final boundary
    RequestPayload.Append((uint8*)TCHAR_TO_UTF8(*BoundarySuffix), BoundarySuffix.Len());

    Request->SetContent(RequestPayload);
    Request->ProcessRequest();
}

void URene_LocalVoiceRecorder::OnUploadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        UE_LOG(LogTemp, Log, TEXT("Voice data uploaded successfully. Server response: %s"), *Response->GetContentAsString());
    }
    else
    {
        FString ErrorReason = TEXT("Unknown error");
        if (Response.IsValid())
        {
            ErrorReason = FString::Printf(TEXT("HTTP %d: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
        }
        UE_LOG(LogTemp, Error, TEXT("Voice data upload failed. Reason: %s"), *ErrorReason);
    }
}
