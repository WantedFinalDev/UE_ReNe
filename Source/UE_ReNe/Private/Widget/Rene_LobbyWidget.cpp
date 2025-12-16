#include "Widget/Rene_LobbyWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Global/Rene_GameInstance.h"
#include "Widget/Rene_ProfileWidget.h"
#include "Widget/Rene_SessionInfoWidget.h"

void URene_LobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Find->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedFind);
	//	btn_CloseList->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedCloseList);
	btn_Create->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedCreate);
	URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance());
	GI->OnFindReneSessionComplete.AddUObject(this, &URene_LobbyWidget::OnFindComplete);
	btn_CreateSession->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedCreateSession);
	btn_BackToMain->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedBack);
	
	//	WBP_ProfileWidget->OnClickDelDoc.AddUObject(this, &URene_LobbyWidget::OnClickedDoc);
	WBP_ProfileWidget->OnClickDashDynamic.AddDynamic(this, &URene_LobbyWidget::OnClickedDash);
}

void URene_LobbyWidget::OnFindComplete(int idx, FString str)
{
	URene_SessionInfoWidget* p_ui = CreateWidget<URene_SessionInfoWidget>(GetWorld(), session_info_widget);
	p_ui->SetSessionInfo(idx, str);
	scr_SessionList->AddChild(p_ui);
}

/*void URene_LobbyWidget::OnClickedCloseList()
{
	// UI를 화면에서 숨기기
	SetVisibility(ESlateVisibility::Collapsed);

	// 입력 모드를 Game Only로 변경
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}
}*/

void URene_LobbyWidget::OnClickedFind()
{
	scr_SessionList->ClearChildren();
	URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance());
	GI->FindReneSession();
}

void URene_LobbyWidget::OnClickedCreate()
{
	Switcher->SetActiveWidgetIndex(1);
	
}

void URene_LobbyWidget::OnClickedCreateSession()
{
	URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance());
	
	GI->CreateReneSession(FCString::Atoi(*etxt_MaxPlayer->GetText().ToString()), etxt_SessionName->GetText().ToString());
	
}

void URene_LobbyWidget::OnClickedBack()
{
	Switcher->SetActiveWidgetIndex(0);
}

void URene_LobbyWidget::OnClickedDash()
{
	Switcher->SetActiveWidgetIndex(2);
}

