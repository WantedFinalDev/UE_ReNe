#include "Widget/Rene_SelectMeetingWidget.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h" // PlayerController 헤더 추가
#include "UE_ReNe.h" // 로그 매크로 사용을 위해 추가
#include "HttpModule.h" // For HTTP requests
#include "Interfaces/IHttpRequest.h" // For HTTP requests
#include "Interfaces/IHttpResponse.h" // For HTTP responses
#include "Serialization/JsonSerializer.h" // For JSON parsing
#include "Dom/JsonObject.h" // For JSON objects
#include "AI/Rene_AI_Interviewer.h" // For ARene_AI_Interviewer
#include "Global/Rene_GameInstance.h" // For network settings
#include "EngineUtils.h" // For TActorIterator
#include "Camera/CameraActor.h" // For ACameraActor

void URene_SelectMeetingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 위젯이 유효한지 확인하고 클릭 이벤트를 바인딩합니다.
	if (btn_StartPrivateInterview)
	{
		btn_StartPrivateInterview->OnClicked.AddDynamic(this, &URene_SelectMeetingWidget::OnStartPrivateInterviewClicked);
	}

	if (btn_StartAIInterview)
	{
		btn_StartAIInterview->OnClicked.AddDynamic(this, &URene_SelectMeetingWidget::OnStartAIInterviewClicked);
	}
	
	btn_Back->OnClicked.AddDynamic(this, &URene_SelectMeetingWidget::OnClickedBackButton);
}

void URene_SelectMeetingWidget::SetTargetActors(AActor* InPrivateInterviewTarget, AActor* InAIInterviewTarget)
{
	PrivateInterviewTargetActor = InPrivateInterviewTarget;
	AIInterviewTargetActor = InAIInterviewTarget;
}

void URene_SelectMeetingWidget::SetActualAIInterviewer(ARene_AI_Interviewer* InAIInterviewer)
{
	ActualAIInterviewer = InAIInterviewer;
	if (ActualAIInterviewer)
	{
		UE_LOG(LogTemp, Log, TEXT("Set Actual AI Interviewer: %s"), *ActualAIInterviewer->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Set Actual AI Interviewer: Null reference provided."));
	}
}

void URene_SelectMeetingWidget::OnClickedBackButton()
{
	OnClickedBack.Broadcast();
	if (IsInViewport())
	{
		RemoveFromParent();
		Cast<ARene_PlayerController>(GetOwningPlayer())->DisableUIControll();
	}
}

void URene_SelectMeetingWidget::OnStartPrivateInterviewClicked()
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer());
	// 내부 변수가 유효한지 확인
	if (PlayerController && PrivateInterviewTargetActor)
	{
		// 1. Move and Sit (Always happens)
		PlayerController->ServerRPC_RequestMoveAndSit(PrivateInterviewTargetActor->GetActorTransform());
		LOGWARNF(TEXT("Requesting move and sit to: %s"), *PrivateInterviewTargetActor->GetActorTransform().ToString());

		// 2. Handle P2P Interview Request Flow
		if (PlayerController->HasAuthority())
		{
			// I am the Host (Server)
			// If I click this, I am either initiating or accepting.
			// For now, let's assume I am accepting if there is a pending request, or just waiting if not.
			// Since we don't have a separate "Accept" button in this widget, we treat this click as "I am ready/Accept".
			PlayerController->ServerRPC_AcceptPrivateInterview();
		}
		else
		{
			// I am the Client
			// I want to request an interview with the Host.
			PlayerController->ServerRPC_RequestPrivateInterview();
		}

		this->RemoveFromParent();
	}
}

void URene_SelectMeetingWidget::OnStartAIInterviewClicked()
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer());
	
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("OnStartAIInterviewClicked: PlayerController is null."));
		return;
	}
	if (!ActualAIInterviewer)
	{
		UE_LOG(LogTemp, Error, TEXT("OnStartAIInterviewClicked: ActualAIInterviewer is null. Please set it via SetActualAIInterviewer."));
		return;
	}
	if (!AIInterviewTargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("OnStartAIInterviewClicked: AIInterviewTargetActor (player movement target) is null."));
		return;
	}

	// --- GameInstance에서 네트워크 설정 가져오기 ---
	URene_GameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URene_GameInstance>() : nullptr;
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AI Interview Start failed: GameInstance is not valid."));
		return;
	}
	const FString AIInterviewStartURL = GameInstance->GetNetworkSettings().AIInterviewStartURL;
	if (AIInterviewStartURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("AI Interview Start failed: AIInterviewStartURL is not set in DataTable."));
		return;
	}
	// --- 설정 가져오기 끝 ---

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &URene_SelectMeetingWidget::OnAIInterviewStartResponse);
	Request->SetURL(AIInterviewStartURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Construct JSON request body
	TSharedPtr<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetNumberField(TEXT("jobseeker_id"), 1); // Placeholder as per API spec
	RequestObj->SetNumberField(TEXT("company_id"), 2);   // Placeholder as per API spec
	RequestObj->SetNumberField(TEXT("job_group_id"), 2); // Placeholder as per API spec

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("Sent AI Interview Start request to: %s"), *AIInterviewStartURL);

	// The existing move and sit logic can remain here, as the AI's initial greeting will play after the server response.
	PlayerController->ServerRPC_TeleportToLocation((PlayerController->GetPawn()->GetActorLocation())+FVector(0,0,440));
	PlayerController->ServerRPC_RequestMoveAndSit(AIInterviewTargetActor->GetActorTransform());
	LOGWARNF(TEXT("Requesting move and sit to: %s"), *AIInterviewTargetActor->GetActorLocation().ToString());
	this->RemoveFromParent();
}

void URene_SelectMeetingWidget::OnAIInterviewStartResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer());
	
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAIInterviewStartResponse: PlayerController is null. Cannot process response."));
		return;
	}
	if (!ActualAIInterviewer)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAIInterviewStartResponse: ActualAIInterviewer is null. Cannot play AI voice."));
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
			FString AIMessage; // For logging or UI display

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
