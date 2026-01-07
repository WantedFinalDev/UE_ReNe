#include "Network/Rene_AIInterviewNetworkManager.h"
#include "Player/Rene_PlayerController.h"
#include "AI/Rene_AI_Interviewer.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "UE_ReNe.h" // 로그 매크로
#include "EngineUtils.h"
#include "Camera/CameraActor.h"

URene_AIInterviewNetworkManager::URene_AIInterviewNetworkManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URene_AIInterviewNetworkManager::CacheInterviewRequest(const FString& URL, int32 UserID, int32 CompanyID, int32 JobGroupID, ARene_AI_Interviewer* Interviewer)
{
	PendingData.StartURL = URL;
	PendingData.UserID = UserID;
	PendingData.CompanyID = CompanyID;
	PendingData.JobGroupID = JobGroupID;
	PendingData.TargetInterviewer = Interviewer;
	PendingData.bIsValid = true;

	UE_LOG(LogTemp, Log, TEXT("AIInterviewManager: Request cached. Waiting for movement completion... UserID: %d, CompanyID: %d"), UserID, CompanyID);
}

void URene_AIInterviewNetworkManager::SendCachedRequest()
{
	if (!PendingData.bIsValid) return;

	UE_LOG(LogTemp, Log, TEXT("AIInterviewManager: Sending cached HTTP request now."));

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &URene_AIInterviewNetworkManager::OnAIInterviewStartResponse);
	Request->SetURL(PendingData.StartURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TSharedPtr<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetNumberField(TEXT("user_id"), PendingData.UserID);
	RequestObj->SetNumberField(TEXT("company_id"), PendingData.CompanyID);
	RequestObj->SetNumberField(TEXT("job_group_id"), PendingData.JobGroupID);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();

	// 데이터 초기화 (중복 전송 방지)
	// 주의: TargetInterviewer는 응답 처리 때 필요하므로 bIsValid만 false로 하고 데이터는 유지하거나, 
	// 응답 핸들러에서 PendingData를 참조하지 않고 멤버 변수로 복사해두는 것이 좋음.
	// 여기서는 단일 요청만 처리한다고 가정하고 bIsValid만 끄고 데이터는 유지함.
	PendingData.bIsValid = false;
}

void URene_AIInterviewNetworkManager::OnAIInterviewStartResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwner());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAIInterviewStartResponse: PlayerController is null."));
		return;
	}

	ARene_AI_Interviewer* ActualAIInterviewer = PendingData.TargetInterviewer.Get();
	if (!ActualAIInterviewer)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAIInterviewStartResponse: ActualAIInterviewer is null or destroyed."));
		return;
	}

	if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FString ResponseBody = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("AI Interview Start successful. Server response: %s"), *ResponseBody);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FString SessionID;
			FString AIAudioBase64;
			FString AIMessage;

			if (JsonObject->TryGetStringField(TEXT("session_id"), SessionID) &&
				JsonObject->TryGetStringField(TEXT("ai_audio_base64"), AIAudioBase64) &&
				JsonObject->TryGetStringField(TEXT("ai_message"), AIMessage))
			{
				PlayerController->SetIsInAIInterview(true);
				PlayerController->SetAISessionID(SessionID);
				
				UE_LOG(LogTemp, Log, TEXT("AI Interview started. SessionID: %s, AI Message: %s"), *SessionID, *AIMessage);

				// Display the initial subtitle message
				PlayerController->DisplayInitialAIMessage(AIMessage);

				// Switch to the interview camera
				ACameraActor* InterviewCamera = nullptr;
				for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
				{
					if (It->ActorHasTag(FName("AIInterviewCamera")))
					{
						InterviewCamera = *It;
						break;
					}
				}

				if (InterviewCamera)
				{
					PlayerController->SetViewTargetWithBlend(InterviewCamera, 0.8f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Could not find ACameraActor with tag 'AIInterviewCamera' in the level."));
				}

				if (!AIAudioBase64.IsEmpty())
				{
					ActualAIInterviewer->PlayAIVoiceResponse(AIAudioBase64);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("AI audio (Base64) field is empty in initial server response."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse session_id, ai_audio_base64, or ai_message from AI Interview Start response."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON from AI Interview Start response. Response: %s"), *ResponseBody);
		}
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
		UE_LOG(LogTemp, Error, TEXT("AI Interview Start failed. Reason: %s"), *ErrorMessage);
	}
}
