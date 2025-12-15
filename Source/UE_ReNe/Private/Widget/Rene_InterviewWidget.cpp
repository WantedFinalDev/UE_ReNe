#include "Widget/Rene_InterviewWidget.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h"

void URene_InterviewWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_End)
	{
		btn_End->OnClicked.AddDynamic(this, &URene_InterviewWidget::OnEndButtonClicked);
	}
}

void URene_InterviewWidget::OnEndButtonClicked()
{
	// 이 위젯을 소유한 PlayerController를 가져와 인터뷰 종료를 요청합니다.
	if (ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->EndInterview();
	}
}
