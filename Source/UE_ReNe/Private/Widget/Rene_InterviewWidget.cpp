#include "Widget/Rene_InterviewWidget.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h"
#include "Components/TextBlock.h"

void URene_InterviewWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_End)
	{
		btn_End->OnClicked.AddDynamic(this, &URene_InterviewWidget::OnEndButtonClicked);
	}

	if (txt_Subtitle)
	{
		txt_Subtitle->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (txt_Loading)
	{
		txt_Loading->SetText(FText::FromString(TEXT("AI 면접자가 다음 질문을 생각 중입니다...")));
		txt_Loading->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (txt_AISpeaking)
	{
		txt_AISpeaking->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (txt_PlayerSpeaking)
	{
		txt_PlayerSpeaking->SetVisibility(ESlateVisibility::Collapsed);
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

void URene_InterviewWidget::UpdateSubtitle(const FString& NewSubtitle)
{
	if (txt_Subtitle)
	{
		txt_Subtitle->SetText(FText::FromString(NewSubtitle));
		txt_Subtitle->SetVisibility(ESlateVisibility::Visible);
	}
}

void URene_InterviewWidget::SetLoadingState(bool bIsLoading)
{
	if (txt_Loading)
	{
		txt_Loading->SetVisibility(bIsLoading ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		// 로딩이 시작되면 이전 자막을 숨겨 UI를 정리
		if (bIsLoading && txt_Subtitle)
		{
			txt_Subtitle->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URene_InterviewWidget::SetInteractivity(bool bIsInteractive)
{
	if (btn_End)
	{
		btn_End->SetIsEnabled(bIsInteractive);
	}
}

void URene_InterviewWidget::ShowAISpeaking(bool bIsSpeaking)
{
	if (txt_AISpeaking)
	{
		if (bIsSpeaking)
		{
			txt_AISpeaking->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			txt_AISpeaking->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URene_InterviewWidget::ShowPlayerSpeaking(bool bIsSpeaking)
{
	if (txt_PlayerSpeaking)
	{
		if (bIsSpeaking)
		{
			txt_PlayerSpeaking->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			txt_PlayerSpeaking->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
