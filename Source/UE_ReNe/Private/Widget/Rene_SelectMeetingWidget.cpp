#include "Widget/Rene_SelectMeetingWidget.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h" // PlayerController 헤더 추가
#include "UE_ReNe.h" // 로그 매크로 사용을 위해 추가

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
}

void URene_SelectMeetingWidget::SetTargetActors(AActor* InPrivateInterviewTarget, AActor* InAIInterviewTarget)
{
	PrivateInterviewTargetActor = InPrivateInterviewTarget;
	AIInterviewTargetActor = InAIInterviewTarget;
}

void URene_SelectMeetingWidget::OnStartPrivateInterviewClicked()
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer());
	// 내부 변수가 유효한지 확인
	if (PlayerController && PrivateInterviewTargetActor)
	{
		PlayerController->ServerRPC_RequestMoveAndSit(PrivateInterviewTargetActor->GetActorTransform());
		LOGWARNF(TEXT("Requesting move and sit to: %s"), *PrivateInterviewTargetActor->GetActorTransform().ToString());
		this->RemoveFromParent();
	}
}

void URene_SelectMeetingWidget::OnStartAIInterviewClicked()
{
	ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer());
	// 내부 변수가 유효한지 확인
	if (PlayerController && AIInterviewTargetActor)
	{
		PlayerController->ServerRPC_TeleportToLocation(AIInterviewTargetActor->GetActorLocation());
		LOGWARNF(TEXT("Requesting teleport to AI Interview Location: %s"), *AIInterviewTargetActor->GetActorLocation().ToString());
		this->RemoveFromParent();
	}
}
