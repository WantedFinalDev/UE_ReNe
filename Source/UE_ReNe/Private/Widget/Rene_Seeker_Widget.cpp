#include "Widget/Rene_Seeker_Widget.h"
#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Player/Rene_PlayerController.h"
#include "Widget/Rene_ProfileWidget.h"


void URene_Seeker_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	pc = Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController());
	btn_Close->OnClicked.AddDynamic(this, &URene_Seeker_Widget::OnClickedClose);
	WBP_ProfileUI->OnClickReturnDynamic.AddDynamic(this, &URene_Seeker_Widget::OnClickedReturn);
	WBP_ProfileUI->OnClickDashDynamic.AddDynamic(this, &URene_Seeker_Widget::OnClickedDash);
}

void URene_Seeker_Widget::OnClickedClose()
{
	if (!pc) return;
	if (!btn_Close) return;
	SetVisibility(ESlateVisibility::Collapsed);
	pc->DisableUIControll();
}

void URene_Seeker_Widget::OnClickedReturn()
{
	if (!pc) return;
	if (pc->IsLocalPlayerController())
	{
		// pc->ClientReturnToMainMenuWithTextReason(FText::GetEmpty());
		// pc->ClientTravel(TEXT("/Game/Maps/StartMap"), TRAVEL_Absolute);
		pc->ConsoleCommand(TEXT("disconnect"));
	}
}

void URene_Seeker_Widget::OnClickedDash()
{
	sw_Main->SetActiveWidgetIndex(1);
	//	Set Seeker Web page URL
}
