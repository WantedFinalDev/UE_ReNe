#include "Widget/Rene_SessionInfoWidget.h"

#include "Components/Button.h"
#include "Global/Rene_GameInstance.h"

void URene_SessionInfoWidget::SetSessionInfo(int32 idx, FString session_name)
{
	info_idx = idx;
	txt_SessionName->SetText(FText::FromString(session_name));
}

void URene_SessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Join->OnClicked.AddDynamic(this, &URene_SessionInfoWidget::OnClickedJoin);
}

void URene_SessionInfoWidget::OnClickedJoin()
{
	URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance());
	GI->JoinReneSession(info_idx);
}

