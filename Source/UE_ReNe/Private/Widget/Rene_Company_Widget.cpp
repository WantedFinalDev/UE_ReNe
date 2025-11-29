#include "Widget/Rene_Company_Widget.h"

#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Global/Rene_Booth_GameState.h"
#include "Widget/Rene_UserListImplementWidget.h"

void URene_Company_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Close->OnClicked.AddDynamic(this, &URene_Company_Widget::OnClickedClose);
	btn_UserList->OnClicked.AddDynamic(this, &URene_Company_Widget::OnClickedList);
	
	if (scr_UserList)
		scr_UserList->SetVisibility(ESlateVisibility::Collapsed);
}

void URene_Company_Widget::OnClickedClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (!pc)
	{
		SHOWERROR()
		return;
	}
	FInputModeGameOnly im;
	pc->SetInputMode(im);
	pc->SetShowMouseCursor(false);
}

void URene_Company_Widget::OnClickedList()
{
	bIsVisibleList = !bIsVisibleList;
	
	if (bIsVisibleList)
	{
		scr_UserList->SetVisibility(ESlateVisibility::Collapsed);
		ClearUserList();
	}
	else
	{
		scr_UserList->SetVisibility(ESlateVisibility::Visible);
		PopulateUserList();
	}
}

void URene_Company_Widget::PopulateUserList()
{
	if (!scr_UserList || !ImplementWidget) return;
	ClearUserList();
	
	TObjectPtr<ARene_Booth_GameState> gs = GetWorld()->GetGameState<ARene_Booth_GameState>();
	if (gs)
	{
		TArray<APlayerState*> players = gs->PlayerArray;
		for (TObjectPtr<APlayerState> player : players)
		{
			if (!player) continue;
			
			// Host 제외
			if (player == GetOwningPlayer()->PlayerState)
				continue;
			
			TObjectPtr<URene_UserListImplementWidget> imp_ui = CreateWidget<URene_UserListImplementWidget>(GetOwningPlayer(), ImplementWidget);
			
			if (imp_ui && p_MeetingPoint)
			{
				imp_ui->SetUserImplementInfo(player);
				
				imp_ui->targetpoint = p_MeetingPoint;
				
				scr_UserList->AddChild(imp_ui);
				
				// 별도 배열 공간 필요시 이용
				// arr_userlistwidget.Add(imp_ui);
			}
			else
			{
				SHOWERRORF("implement UI OR TargetPoint MIA")
				return;
			}
		}
	}
}

void URene_Company_Widget::ClearUserList()
{
	if (!scr_UserList)
		return;
	scr_UserList->ClearChildren();
	// arr_userlistwidget.Empty();
}
