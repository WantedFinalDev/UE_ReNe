#include "Widget/Rene_StartWidget.h"

#include "Components/Button.h"
#include "Global/Rene_GameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void URene_StartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	
	btn_Enter->OnClicked.AddDynamic(this, &URene_StartWidget::OnClickedEnter);
	btn_Exit->OnClicked.AddDynamic(this, &URene_StartWidget::OnClickedExit);
}

void URene_StartWidget::OnClickedEnter()
{
	
}

void URene_StartWidget::OnClickedExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
