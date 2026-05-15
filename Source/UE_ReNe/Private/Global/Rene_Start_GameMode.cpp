#include "Global/Rene_Start_GameMode.h"

#include "UE_ReNe.h"
#include "Widget/Rene_StartWidget.h"
#include "Blueprint/UserWidget.h"

/*
 *	12.10 수 16:49 비둘기
 *	TODO : Lobby Game Mode 내용 이관중
 *	
 */

void ARene_Start_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// checkf(IsValid(StartUIClass), TEXT("WBP_StartUI is Empty"));
	
}

void ARene_Start_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	LOGWARN();
	
	/*
	NewPlayer->bShowMouseCursor = true;
	FInputModeUIOnly im;
	NewPlayer->SetInputMode(im);
	
	StartUI = CreateWidget<URene_StartWidget>(GetWorld(), StartUIClass);
	StartUI->AddToViewport();
	*/
}
