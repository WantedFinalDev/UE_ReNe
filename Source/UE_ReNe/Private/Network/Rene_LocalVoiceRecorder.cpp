// Rene_LocalVoiceRecorder.cpp

#include "Network/Rene_LocalVoiceRecorder.h"
#include "Modules/ModuleManager.h"
#include "VoiceModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Controller.h"
#include "Global/Rene_GameInstance.h"
#include "Player/Rene_PlayerController.h"
#include "AI/Rene_AI_Interviewer.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "EngineUtils.h"

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
    
    {
        FScopeLock Lock(&VoiceBufferCriticalSection);
        RecordedVoiceData.Empty();
    }

    if (VoiceCapture->Start())
    {
        UE_LOG(LogTemp, Log, TEXT("Local voice recording started."));

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
            RecordedVoiceData.Append(TempBuffer.GetData(), BytesRead);
        }
    }
}

void URene_LocalVoiceRecorder::StopAndUploadRecording(const FString& AISessionID)
{
    if (!bIsRecording)
    {
        return;
    }

    bIsRecording = false;
    
    GetWorld()->GetTimerManager().ClearTimer(VoiceCaptureTimerHandle);
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
            // 로딩 상태를 다시 false로 되돌려야 UI가 멈추지 않음
            OnAIResponseStateChanged.Broadcast(false);
            return;
        }
        VoiceDataToUpload = RecordedVoiceData;
        RecordedVoiceData.Empty();
    }

    UE_LOG(LogTemp, Log, TEXT("Uploading %d bytes of voice data."), VoiceDataToUpload.Num());

    FString PlayerName = TEXT("UnknownPlayer");
    
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
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find Local PlayerController (Index 0)."));
    }

    OnAIResponseStateChanged.Broadcast(true);
    SendHttpRequest(VoiceDataToUpload, PlayerName, AISessionID);
}

void URene_LocalVoiceRecorder::SendHttpRequest(const TArray<uint8>& VoiceData, const FString& PlayerName, const FString& AISessionID)
{
    URene_GameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URene_GameInstance>() : nullptr;
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Voice upload failed: GameInstance is not valid."));
        OnUploadFailure.Broadcast(TEXT("GameInstance is not valid."));
        OnAIResponseStateChanged.Broadcast(false);
        return;
    }
    
    FString TargetURL;
    if (!AISessionID.IsEmpty())
    {
        TargetURL = GameInstance->GetNetworkSettings().AIInterviewChatURL;
        if (TargetURL.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("AI Voice Chat upload failed: AIInterviewChatURL is not set in DataTable."));
            OnUploadFailure.Broadcast(TEXT("AIInterviewChatURL is not set in DataTable."));
            OnAIResponseStateChanged.Broadcast(false);
            return;
        }
    }
    else
    {
        TargetURL = GameInstance->GetNetworkSettings().VoiceDataUploadURL;
        if (TargetURL.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("Voice upload failed: VoiceDataUploadURL is not set in DataTable."));
            OnUploadFailure.Broadcast(TEXT("VoiceDataUploadURL is not set in DataTable."));
            OnAIResponseStateChanged.Broadcast(false);
            return;
        }
    }

    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &URene_LocalVoiceRecorder::OnUploadComplete);
    Request->SetURL(TargetURL);
    Request->SetVerb(TEXT("POST"));

    FString SpeakerRole;
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        SpeakerRole = TEXT("company");
    }
    else
    {
        SpeakerRole = TEXT("seeker");
    }

    FString Boundary = FString::Printf(TEXT("rene-voice-boundary-%s"), *FGuid::NewGuid().ToString());

    Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *Boundary));

    TArray<uint8> RequestPayload;
    
    auto AddStringField = [&](const FString& InString)
    {
        FTCHARToUTF8 Convert(*InString);
        RequestPayload.Append((uint8*)Convert.Get(), Convert.Length());
    };

    const FString BoundaryLine = TEXT("--") + Boundary + TEXT("\r\n");
    const FString BoundaryEnd = TEXT("\r\n--") + Boundary + TEXT("--\r\n");

    AddStringField(BoundaryLine);
    
    FString SpeakerRoleHeader = FString::Printf(TEXT("Content-Disposition: form-data; name=\"speaker_role\"\r\n\r\n"));
    AddStringField(SpeakerRoleHeader);
    AddStringField(SpeakerRole);
    AddStringField(TEXT("\r\n"));

    if (!AISessionID.IsEmpty())
    {
        AddStringField(BoundaryLine);
        FString SessionIDHeader = FString::Printf(TEXT("Content-Disposition: form-data; name=\"session_id\"\r\n\r\n"));
        AddStringField(SessionIDHeader);
        AddStringField(AISessionID);
        AddStringField(TEXT("\r\n"));
    }

    FString Timestamp = FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S"));
    FString UniqueFileName = FString::Printf(TEXT("%s_%s_%s.pcm"), *SpeakerRole, *PlayerName, *Timestamp);

    AddStringField(BoundaryLine);

    FString FileHeader = FString::Printf(TEXT("Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n"), *UniqueFileName);
    FileHeader += TEXT("Content-Type: application/octet-stream\r\n\r\n");
    
    AddStringField(FileHeader);

    RequestPayload.Append(VoiceData);

    AddStringField(BoundaryEnd);

    Request->SetContent(RequestPayload);
    Request->ProcessRequest();
}

void URene_LocalVoiceRecorder::OnUploadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    OnAIResponseStateChanged.Broadcast(false);

    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString ResponseBody = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Voice data uploaded successfully. Server response: %s"), *ResponseBody);
        OnUploadSuccess.Broadcast(ResponseBody);

        ProcessAIResponse(ResponseBody);
    }
    else
    {
        FString ErrorMessageForLog = TEXT("Unknown error");
        FString ErrorMessageForUI = TEXT("오류가 발생했습니다. 다시 시도해주세요.");

        if (Response.IsValid())
        {
            FString ResponseBody = Response->GetContentAsString();
            ErrorMessageForLog = FString::Printf(TEXT("HTTP %d: %s"), Response->GetResponseCode(), *ResponseBody);

            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                FString DetailMessage;
                if (JsonObject->TryGetStringField(TEXT("detail"), DetailMessage) && !DetailMessage.IsEmpty())
                {
                    ErrorMessageForUI = DetailMessage;
                }
            }
        }
        else
        {
            ErrorMessageForLog = TEXT("HTTP request failed or no response received.");
        }

        OnAIMessageReceived.Broadcast(ErrorMessageForUI);

        UE_LOG(LogTemp, Error, TEXT("Voice data upload failed. Reason: %s"), *ErrorMessageForLog);
        OnUploadFailure.Broadcast(ErrorMessageForLog);
    }
}

void URene_LocalVoiceRecorder::ProcessAIResponse(const FString& ResponseBody)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FString AIMessage;
        if (JsonObject->TryGetStringField(TEXT("ai_message"), AIMessage) && !AIMessage.IsEmpty())
        {
            OnAIMessageReceived.Broadcast(AIMessage);
        }

        FString AIAudioBase64;
        if (JsonObject->TryGetStringField(TEXT("ai_audio_base64"), AIAudioBase64))
        {
            if (!AIAudioBase64.IsEmpty())
            {
                UE_LOG(LogTemp, Log, TEXT("Received AI audio (Base64). Attempting to play."));

                ARene_AI_Interviewer* AIInterviewer = nullptr;
                for (TActorIterator<ARene_AI_Interviewer> It(GetWorld()); It; ++It)
                {
                    AIInterviewer = *It;
                    break;
                }

                if (AIInterviewer)
                {
                    AIInterviewer->PlayAIVoiceResponse(AIAudioBase64);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Could not find ARene_AI_Interviewer in the world to play AI voice."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AI audio (Base64) field is empty in server response."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Server response does not contain 'ai_audio_base64' field."));
        }

        FString InterviewStatus;
        if (JsonObject->TryGetStringField(TEXT("status"), InterviewStatus))
        {
            if (InterviewStatus.Equals(TEXT("done"), ESearchCase::IgnoreCase))
            {
                int32 InterviewResultID = -1;
                if (JsonObject->TryGetNumberField(TEXT("interview_result_id"), InterviewResultID))
                {
                    UE_LOG(LogTemp, Log, TEXT("AI Interview is DONE. Result ID: %d"), InterviewResultID);
                    OnAIInterviewFinished.Broadcast(InterviewResultID);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("AI Interview is DONE, but 'interview_result_id' was not found in the response."));
                }

                if (APlayerController* OwningPC = Cast<APlayerController>(GetOwner()))
                {
                    if (ARene_PlayerController* RenePC = Cast<ARene_PlayerController>(OwningPC))
                    {
                        RenePC->SetIsInAIInterview(false);
                        RenePC->SetAISessionID(FString());
                    }
                }
                return; 
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("AI Interview Status: %s"), *InterviewStatus);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Server response does not contain 'status' field."));
        }

        FString InterviewStage;
        if (JsonObject->TryGetStringField(TEXT("interview_stage"), InterviewStage))
        {
            if (!InterviewStage.IsEmpty())
            {
                OnInterviewStageReceived.Broadcast(InterviewStage);
                UE_LOG(LogTemp, Log, TEXT("Received Interview Stage: %s"), *InterviewStage);
            }
        }

        FString NewSessionID;
        if (JsonObject->TryGetStringField(TEXT("session_id"), NewSessionID))
        {
            if (!NewSessionID.IsEmpty())
            {
                if (APlayerController* OwningPC = Cast<APlayerController>(GetOwner()))
                {
                    if (ARene_PlayerController* RenePC = Cast<ARene_PlayerController>(OwningPC))
                    {
                        RenePC->SetAISessionID(NewSessionID);
                        UE_LOG(LogTemp, Log, TEXT("Updated AI Session ID to: %s"), *NewSessionID);
                    }
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize server response JSON for AI voice. Response: %s"), *ResponseBody);
    }
}

void URene_LocalVoiceRecorder::RequestForceEndInterview(const FString& AISessionID)
{
    URene_GameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URene_GameInstance>() : nullptr;
    if (!GameInstance) return;

    FString TargetURL = GameInstance->GetNetworkSettings().AIInterviewChatURL;
    if (TargetURL.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Force End failed: AIInterviewChatURL is not set."));
        return;
    }
    
    TargetURL = TargetURL.Replace(TEXT("/chat/voice"), TEXT("/force-end"));

    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &URene_LocalVoiceRecorder::OnForceEndComplete);
    Request->SetURL(TargetURL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonRequest = MakeShareable(new FJsonObject);
    JsonRequest->SetStringField(TEXT("session_id"), AISessionID);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);
    Request->ProcessRequest();
    
    OnAIResponseStateChanged.Broadcast(true);
}

void URene_LocalVoiceRecorder::OnForceEndComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    OnAIResponseStateChanged.Broadcast(false);

    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString ResponseBody = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Force End successful. Response: %s"), *ResponseBody);
        
        ProcessAIResponse(ResponseBody);
    }
    else
    {
        FString ErrorMsg = TEXT("인터뷰 종료 요청 실패.");
        if (Response.IsValid())
        {
            ErrorMsg += FString::Printf(TEXT(" (HTTP %d)"), Response->GetResponseCode());
        }
        OnAIMessageReceived.Broadcast(ErrorMsg);
        UE_LOG(LogTemp, Error, TEXT("Force End failed. %s"), *ErrorMsg);
    }
}
