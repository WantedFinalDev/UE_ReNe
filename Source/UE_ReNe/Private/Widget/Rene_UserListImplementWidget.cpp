#include "Widget/Rene_UserListImplementWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void URene_UserListImplementWidget::SetUserImplementInfo(APlayerState* ps)
{
	if (!ps || !txt_UserName) return;
	txt_UserName->SetText(FText::FromString(ps->GetPlayerName()));
	playerstate = ps;
}

void URene_UserListImplementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_P2P->OnClicked.AddDynamic(this, &URene_UserListImplementWidget::OnClickedP2P);
}

void URene_UserListImplementWidget::OnClickedP2P()
{
	
}
