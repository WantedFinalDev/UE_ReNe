
#include "Widget/Rene_UploadingPopupWidget.h"

#include "Components/WidgetSwitcher.h"

void URene_UploadingPopupWidget::ShowUploadingState()
{
	Switcher->SetActiveWidgetIndex(0);
}

void URene_UploadingPopupWidget::ShowCompleteState()
{
	Switcher->SetActiveWidgetIndex(1);
	SetHideTimer();
}

void URene_UploadingPopupWidget::ShowErrorState()
{
	Switcher->SetActiveWidgetIndex(2);
	SetHideTimer();
}

void URene_UploadingPopupWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

void URene_UploadingPopupWidget::HideWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void URene_UploadingPopupWidget::SetHideTimer()
{
	if (!GetWorld()) return;

	// 기존 타이머가 있으면 취소
	if (GetWorld()->GetTimerManager().IsTimerActive(HideTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this,
		&URene_UploadingPopupWidget::HideWidget, 2.0f, false);
}
