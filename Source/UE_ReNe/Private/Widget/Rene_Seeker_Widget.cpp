#include "Widget/Rene_Seeker_Widget.h"
#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Global/Rene_GameInstance.h"
#include "Player/Rene_PlayerController.h"
#include "Widget/Rene_ProfileWidget.h"
#include "Widget/Rene_DashBoardWidget.h" // 헤더 추가


void URene_Seeker_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	pc = Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController());
	btn_Close->OnClicked.AddDynamic(this, &URene_Seeker_Widget::OnClickedClose);
	btn_DashToMain->OnClicked.AddDynamic(this, &URene_Seeker_Widget::OnClickedDashToMain);
	WBP_ProfileUI->OnClickReturnDynamic.AddDynamic(this, &URene_Seeker_Widget::OnClickedReturn);
	WBP_ProfileUI->OnClickDashDynamic.AddDynamic(this, &URene_Seeker_Widget::OnClickedDash);
}

void URene_Seeker_Widget::OnClickedClose()
{
	if (!pc) return;
	if (!btn_Close) return;
	SetVisibility(ESlateVisibility::Collapsed);
	pc->DisableUIControll();
	pc->ShowHUD();
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
	
	// [추가] 대시보드 위젯을 찾아 URL 새로고침
	if (UWidget* ActiveWidget = sw_Main->GetActiveWidget())
	{
		if (URene_DashBoardWidget* DashBoardWidget = Cast<URene_DashBoardWidget>(ActiveWidget))
		{
			DashBoardWidget->RefreshDashboardURL();
		}
	}
}

void URene_Seeker_Widget::OnClickedDashToMain()
{
	sw_Main->SetActiveWidgetIndex(0);
}
