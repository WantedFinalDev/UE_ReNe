#include "Global/Rene_Lobby_GameMode.h"
#include "UE_ReNe.h"
#include "UE_ReNePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Player/Rene_PlayerController.h"
#include "Widget/Rene_LobbyWidget.h"

void ARene_Lobby_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARene_Lobby_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	checkf(IsValid(LobbyUIClass), TEXT("LobbyUIClass must be set in BP_Lobby_GameMode!"));
	
	LobbyUI = CreateWidget<URene_LobbyWidget>(NewPlayer, LobbyUIClass);
	LobbyUI->AddToViewport();
	
	NewPlayer->bShowMouseCursor = true;
	FInputModeUIOnly im;
	NewPlayer->SetInputMode(im);
}




