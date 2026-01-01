#include "Widget/Rene_HUD.h"

#include "Components/Button.h"

void URene_HUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Home->OnClicked.AddDynamic(this, &URene_HUD::OnClickedHome);
}

void URene_HUD::OnClickedHome()
{
	//	TODO : Go to Info UI
}
