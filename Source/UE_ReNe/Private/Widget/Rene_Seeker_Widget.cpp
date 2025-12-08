#include "Widget/Rene_Seeker_Widget.h"
#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h"


void URene_Seeker_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_Close->OnClicked.AddDynamic(this, &URene_Seeker_Widget::OnClickedClose);
	
}

void URene_Seeker_Widget::OnClickedClose()
{
	if (!btn_Close) return;
	SetVisibility(ESlateVisibility::Collapsed);
	
	TObjectPtr<ARene_PlayerController> pc = Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (!pc) return;
	
	pc->DisableUIControll();
}




