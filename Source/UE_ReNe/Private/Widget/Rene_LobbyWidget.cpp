#include "Widget/Rene_LobbyWidget.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Global/Rene_GameInstance.h"
#include "Widget/Rene_SessionInfoWidget.h"

void URene_LobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Find->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedFind);
	btn_CloseList->OnClicked.AddDynamic(this, &URene_LobbyWidget::OnClickedCloseList);
	URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance());
	GI->OnFindReneSessionComplete.AddUObject(this, &URene_LobbyWidget::OnFindComplete);
}

void URene_LobbyWidget::OnFindComplete(int idx, FString str)
{
	URene_SessionInfoWidget* p_ui = CreateWidget<URene_SessionInfoWidget>(GetWorld(), session_info_widget);
	scr_SessionList->AddChild(p_ui);
}

void URene_LobbyWidget::OnClickedCloseList()
{
	UE_LOG(LogTemp, Warning, TEXT("Close List"))
}

void URene_LobbyWidget::OnClickedFind()
{
	URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance());
	GI->FindReneSession();
}
