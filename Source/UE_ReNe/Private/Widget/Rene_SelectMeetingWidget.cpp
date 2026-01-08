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
#include "Global/Rene_PlayerState.h" // [추가] PlayerState 접근용
#include "GameFramework/GameStateBase.h" // [추가] GameState 접근용
#include "EngineUtils.h" // For TActorIterator
#include "Camera/CameraActor.h" // For ACameraActor

void URene_SelectMeetingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 중복 바인딩 방지: 기존 바인딩 제거 후 재바인딩
	if (btn_StartPrivateInterview)
	{
		btn_StartPrivateInterview->OnClicked.RemoveDynamic(this, &URene_SelectMeetingWidget::OnStartPrivateInterviewClicked);
		btn_StartPrivateInterview->OnClicked.AddDynamic(this, &URene_SelectMeetingWidget::OnStartPrivateInterviewClicked);
	}

	if (btn_StartAIInterview)
	{
		btn_StartAIInterview->OnClicked.RemoveDynamic(this, &URene_SelectMeetingWidget::OnStartAIInterviewClicked);
		btn_StartAIInterview->OnClicked.AddDynamic(this, &URene_SelectMeetingWidget::OnStartAIInterviewClicked);
	}

	if (btn_Back)
	{
		btn_Back->OnClicked.RemoveDynamic(this, &URene_SelectMeetingWidget::OnClickedBackButton);
		btn_Back->OnClicked.AddDynamic(this, &URene_SelectMeetingWidget::OnClickedBackButton);
	}
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

// [변경] 호스트 정보 자동 탐색으로 인해 더 이상 필수 아님. (호환성 유지 위해 남겨둠)
void URene_SelectMeetingWidget::SetInterviewDetails(int32 InCompanyID, int32 InJobGroupID)
{
	TargetCompanyID = InCompanyID;
	TargetJobGroupID = InJobGroupID;
	UE_LOG(LogTemp, Log, TEXT("Interview Details Set (Manual Override): CompanyID=%d, JobGroupID=%d"), TargetCompanyID, TargetJobGroupID);
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
		// To Infodesk UI
		OnClickedInterview.Broadcast();
		
		PlayerController->ServerRPC_TeleportToLocation(FVector(100,510,119));
		
		// 1. Move and Sit (Always happens)
		PlayerController->ServerRPC_RequestMoveAndSit(PrivateInterviewTargetActor->GetActorTransform());
		LOGWARNF(TEXT("Requesting move and sit to: %s"), *PrivateInterviewTargetActor->GetActorTransform().ToString());

		// Delay camera switch to allow player to see movement start
		const float CameraSwitchDelay = 2.5f; // << 여기에서 지연 시간을 초 단위로 조절할 수 있습니다.
		FTimerHandle DummyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DummyTimerHandle, this, &URene_SelectMeetingWidget::SwitchToPrivateInterviewCamera_Delayed, CameraSwitchDelay, false);

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

void URene_SelectMeetingWidget::SwitchToPrivateInterviewCamera_Delayed()
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer());
	if (!PlayerController)
	{
		return;
	}

	ACameraActor* PrivateCamera = nullptr;
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(FName("PrivateInterviewCamera")))
		{
			PrivateCamera = *It;
			break;
		}
	}

	if (PrivateCamera)
	{
		UE_LOG(LogTemp, Log, TEXT("Switching to PrivateInterviewCamera after delay."));
		PlayerController->SetViewTargetWithBlend(PrivateCamera, 0.8f);
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

	// [수정 시작] 데이터 가져오기 로직
	
	// 1. user_id: 내 PlayerState에서 가져옴 (로그인된 구직자 ID)
	int32 RequestUserID = 0;
	if (ARene_PlayerState* MyPS = PlayerController->GetPlayerState<ARene_PlayerState>())
	{
		RequestUserID = FCString::Atoi(*MyPS->GetReneUserId());
	}
	
	if (RequestUserID == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnStartAIInterviewClicked: UserID is 0. User might not be logged in."));
	}

	// 2. company_id, job_group_id: 호스트(Server) PlayerState에서 가져옴
	int32 HostCompanyID = 0;
	int32 HostJobGroupID = 0;

	// 수동 설정값이 있으면 우선 사용 (테스트 목적 등)
	if (TargetCompanyID > 0 && TargetJobGroupID > 0)
	{
		HostCompanyID = TargetCompanyID;
		HostJobGroupID = TargetJobGroupID;
	}
	else
	{
		// GameState를 통해 모든 플레이어 순회
		if (AGameStateBase* GameState = GetWorld()->GetGameState())
		{
			for (APlayerState* PS : GameState->PlayerArray)
			{
				if (ARene_PlayerState* RenePS = Cast<ARene_PlayerState>(PS))
				{
					FReneUserData UserData = RenePS->GetReneUserData();
					if (UserData.Role.Equals(TEXT("company"), ESearchCase::IgnoreCase))
					{
						HostCompanyID = FCString::Atoi(*UserData.ID);
						HostJobGroupID = UserData.JobGroupID;
						UE_LOG(LogTemp, Log, TEXT("Found Host Company: ID=%d, JobGroup=%d, Name=%s"), HostCompanyID, HostJobGroupID, *UserData.Name);
						break; // 호스트를 찾았으므로 중단
					}
				}
			}
		}
	}

	// Fallback
	const int32 FALLBACK_ID = 4; 
	int32 FinalCompanyID = (HostCompanyID > 0) ? HostCompanyID : FALLBACK_ID;
	int32 FinalJobGroupID = (HostJobGroupID > 0) ? HostJobGroupID : FALLBACK_ID;

	if (HostCompanyID == 0 || HostJobGroupID == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnStartAIInterviewClicked: Could not find Host Company info. Using fallback values (%d, %d)."), FinalCompanyID, FinalJobGroupID);
	}
	// [수정 끝]

	// [변경] 직접 HTTP 요청 및 이동 호출 -> PC에 위임
	PlayerController->RequestAIInterviewStart(AIInterviewStartURL, RequestUserID, FinalCompanyID, FinalJobGroupID, AIInterviewTargetActor, ActualAIInterviewer);
	
	//	To Infodesk UI
	OnClickedInterview.Broadcast();

	this->RemoveFromParent();
}

void URene_SelectMeetingWidget::OnAIInterviewStartResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// 이 함수는 이제 사용되지 않지만, 델리게이트 바인딩이 없으므로 안전함.
	// 추후 삭제 가능.
}
