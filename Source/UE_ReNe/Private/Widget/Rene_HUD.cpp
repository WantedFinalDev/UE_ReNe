#include "Widget/Rene_HUD.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Player/Rene_PlayerController.h"

void URene_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayer())
		p_OwnerController = Cast<ARene_PlayerController>(GetOwningPlayer());

	// 중복 바인딩 방지: 기존 바인딩 제거 후 재바인딩
	btn_Home->OnClicked.RemoveDynamic(this, &URene_HUD::OnClickedHome);
	btn_HQ_Profile->OnClicked.RemoveDynamic(this, &URene_HUD::OnClickedProfile);
	btn_HQ_Meeting->OnClicked.RemoveDynamic(this, &URene_HUD::OnClickedMeeting);
	btn_HQ_Close->OnClicked.RemoveDynamic(this, &URene_HUD::OnClickedClose);

	btn_Home->OnClicked.AddDynamic(this, &URene_HUD::OnClickedHome);
	btn_HQ_Profile->OnClicked.AddDynamic(this, &URene_HUD::OnClickedProfile);
	btn_HQ_Meeting->OnClicked.AddDynamic(this, &URene_HUD::OnClickedMeeting);
	btn_HQ_Close->OnClicked.AddDynamic(this, &URene_HUD::OnClickedClose);
}

void URene_HUD::OnClickedHome()
{
	sw_HUD->SetActiveWidgetIndex(1);
	OnClickedHomeDelegate.Broadcast();
	p_OwnerController->EnableUIControll();
}

void URene_HUD::OnClickedProfile()
{
	p_OwnerController->OnToggleMenu();
	RemoveFromParent();
}

void URene_HUD::OnClickedMeeting()
{
	p_OwnerController->ShowInfodeskUI();
	RemoveFromParent();
}

void URene_HUD::OnClickedClose()
{
	sw_HUD->SetActiveWidgetIndex(0);
	p_OwnerController->DisableUIControll();
}


