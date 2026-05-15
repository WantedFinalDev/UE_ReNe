#include "Widget_Join_conv.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Global/Rene_GameInstance.h"
#include "Widget/Rene_SessionInfoWidget.h"

void UWidget_Join_conv::OnFindComplete(int idx, FString str)
{
	if (!session_info_widget || !scr_SessionList) return;
	URene_SessionInfoWidget* p_ui = CreateWidget<URene_SessionInfoWidget>(GetWorld(), session_info_widget);
	if (!p_ui) return;
	p_ui->SetSessionInfo(idx, str);
	scr_SessionList->AddChild(p_ui);
}

void UWidget_Join_conv::OnClickedFind()
{
	if (!scr_SessionList) return;
	scr_SessionList->ClearChildren();
	if (URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance()))
		GI->FindReneSession();
}

void UWidget_Join_conv::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (btn_Find)
		btn_Find->OnClicked.AddDynamic(this, &UWidget_Join_conv::OnClickedFind);

	if (URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance()))
		GI->OnFindReneSessionComplete.AddUObject(this, &UWidget_Join_conv::OnFindComplete);
}

void UWidget_Join_conv::NativeDestruct()
{
	if (URene_GameInstance* GI = Cast<URene_GameInstance>(GetGameInstance()))
		GI->OnFindReneSessionComplete.RemoveAll(this);
	Super::NativeDestruct();
}
