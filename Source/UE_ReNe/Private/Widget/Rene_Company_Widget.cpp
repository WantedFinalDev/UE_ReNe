#include "Widget/Rene_Company_Widget.h"

#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Global/Rene_Booth_GameState.h"
#include "Global/Rene_GameInstance.h"
#include "Player/Rene_PlayerController.h"
#include "Global/Rene_PlayerState.h"
#include "Widget/Rene_ProfileWidget.h"
#include "Widget/Rene_UserListImplementWidget.h"
#include "Widget/Rene_DashBoardWidget.h" // 헤더 추가

void URene_Company_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	// 중복 바인딩 방지: 기존 바인딩 제거 후 재바인딩
	btn_HostUIClose->OnClicked.RemoveDynamic(this, &URene_Company_Widget::OnClickedClose);
	btn_DashToMain->OnClicked.RemoveDynamic(this, &URene_Company_Widget::OnClickedDashToMain);
	WBP_ProfileUI->OnClickReturnDynamic.RemoveDynamic(this, &URene_Company_Widget::OnClickedReturn);
	WBP_ProfileUI->OnClickDashDynamic.RemoveDynamic(this, &URene_Company_Widget::OnClickedMainToDash);

	btn_HostUIClose->OnClicked.AddDynamic(this, &URene_Company_Widget::OnClickedClose);
	btn_DashToMain->OnClicked.AddDynamic(this, &URene_Company_Widget::OnClickedDashToMain);
	WBP_ProfileUI->OnClickReturnDynamic.AddDynamic(this, &URene_Company_Widget::OnClickedReturn);
	WBP_ProfileUI->OnClickDashDynamic.AddDynamic(this, &URene_Company_Widget::OnClickedMainToDash);
}

void URene_Company_Widget::OnClickedClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	ARene_PlayerController* pc = Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (!pc)
	{
		LOGERROR()
		return;
	}
	pc->DisableUIControll();
	pc->ShowHUD();
}

void URene_Company_Widget::OnClickedMainToDash()
{
	if (!IsValid(sw_Switcher)) return;
	
	sw_Switcher->SetActiveWidgetIndex(1);

	// [추가] 대시보드 위젯을 찾아 URL 새로고침 (로그 추가)
	if (UWidget* ActiveWidget = sw_Switcher->GetActiveWidget())
	{
		if (URene_DashBoardWidget* DashBoardWidget = Cast<URene_DashBoardWidget>(ActiveWidget))
		{
			UE_LOG(LogTemp, Log, TEXT("CompanyWidget: Found DashBoardWidget. Refreshing URL..."));
			DashBoardWidget->RefreshDashboardURL();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CompanyWidget: Active widget is NOT URene_DashBoardWidget. Class: %s"), *ActiveWidget->GetClass()->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CompanyWidget: No active widget found in switcher."));
	}
}

void URene_Company_Widget::OnClickedDashToMain()
{
	sw_Switcher->SetActiveWidgetIndex(0);
}

void URene_Company_Widget::OnClickedReturn()
{
	TObjectPtr<ARene_PlayerController> pc = Cast<ARene_PlayerController>(GetOwningPlayer());
	TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(GetWorld()->GetGameInstance());
	if (pc && pc->HasAuthority())
	{
		if (gi)
			gi->DestroyReneSession();
		
		GetWorld()->ServerTravel(TEXT("/Game/Maps/StartMap"));
	}
}

/*void URene_Company_Widget::PopulateUserList()
{
	if (!scr_UserList || !ImplementWidget) return;
	ClearUserList();
	
	TObjectPtr<ARene_Booth_GameState> gs = GetWorld()->GetGameState<ARene_Booth_GameState>();
	if (gs)
	{
		TArray<TObjectPtr<ARene_PlayerState>> arr_players = gs->Rene_PlayerArray;
		for (TObjectPtr<ARene_PlayerState> ps : arr_players)
		{
			if (!ps) continue;
			
			// Host 제외
			APlayerController* localpc = GetOwningPlayer();		
			if (localpc && (ps == localpc->GetPlayerState<ARene_PlayerState>()))
				continue;
			
			TObjectPtr<URene_UserListImplementWidget> imp_ui = CreateWidget<URene_UserListImplementWidget>(GetOwningPlayer(), ImplementWidget);
			
			if (imp_ui)
			{
				imp_ui->SetUserImplementInfo(ps);
				scr_UserList->AddChild(imp_ui);
				
				// 별도 배열 공간 필요시 이용
				// arr_userlistwidget.Add(imp_ui);
			}
			else
			{
				LOGERRORF(TEXT("implement UI OR TargetPoint MIA"))
				return;
			}
		}
	}
}*/

/*void URene_Company_Widget::ClearUserList()
{
	if (!scr_UserList) return;
	scr_UserList->ClearChildren();
	// arr_userlistwidget.Empty();
}*/
