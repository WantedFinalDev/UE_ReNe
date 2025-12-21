#include "Widget/Rene_InterviewResultPopupWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h" // PlayerController 헤더 추가

void URene_InterviewResultPopupWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (btn_ShowReport)
    {
        btn_ShowReport->OnClicked.AddDynamic(this, &URene_InterviewResultPopupWidget::HandleShowReportClicked);
    }

    if (btn_Close)
    {
        btn_Close->OnClicked.AddDynamic(this, &URene_InterviewResultPopupWidget::HandleCloseClicked);
    }
}

void URene_InterviewResultPopupWidget::SetResultID(int32 InResultID)
{
    if (txt_ResultID)
    {
        txt_ResultID->SetText(FText::AsNumber(InResultID));
    }
}

void URene_InterviewResultPopupWidget::HandleShowReportClicked()
{
    OnShowReportClicked.Broadcast();
}

void URene_InterviewResultPopupWidget::HandleCloseClicked()
{
    // Get the owning PlayerController
    if (ARene_PlayerController* PlayerController = Cast<ARene_PlayerController>(GetOwningPlayer()))
    {
        // Call the PlayerController's function to close both widgets
        PlayerController->CloseReportAndWebView();
    }
}
