#include "Global/Rene_Start_GameMode.h"

#include "UE_ReNe.h"
#include "Widget/Rene_StartWidget.h"
#include "Blueprint/UserWidget.h"

void ARene_Start_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(IsValid(StartUIClass), TEXT("WBP_StartUI is Empty"));
	
}

void ARene_Start_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	SHOWWARN();
	
	NewPlayer->bShowMouseCursor = true;
	FInputModeUIOnly im;
	NewPlayer->SetInputMode(im);
	
	StartUI = CreateWidget<URene_StartWidget>(GetWorld(), StartUIClass);
	StartUI->AddToViewport();
}
