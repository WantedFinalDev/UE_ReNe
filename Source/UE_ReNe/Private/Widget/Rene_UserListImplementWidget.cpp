#include "Widget/Rene_UserListImplementWidget.h"

#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerState.h"
#include "Global/Rene_PlayerState.h"
#include "Player/Rene_PlayerController.h"

void URene_UserListImplementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Profile->OnClicked.AddDynamic(this, &URene_UserListImplementWidget::OnClickedProfile);
}

void URene_UserListImplementWidget::SetUserImplementInfo(ARene_PlayerState* ps)
{
	if (!ps || !txt_UserName) return;
	txt_UserName->SetText(FText::FromString(ps->GetReneUserName()));
	thisps = ps;
}

void URene_UserListImplementWidget::OnClickedProfile()
{
	//	TODO : Get Profile Request to Server
	LOGWARNF(TEXT("Profile Button Clicked"))
}

