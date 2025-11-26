#include "Global/Rene_Lobby_GameMode.h"
#include "UE_ReNe.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Rene_LobbyWidget.h"

void ARene_Lobby_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARene_Lobby_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	SHOWWARN();
	
	checkf(IsValid(LobbyUIClass), TEXT("LobbyUIClass must be set in BP_Lobby_GameMode!"));
	
	LobbyUI = CreateWidget<URene_LobbyWidget>(NewPlayer, LobbyUIClass);
	LobbyUI->AddToViewport();
	
	EnableUIControll();
}

void ARene_Lobby_GameMode::EnableUIControll()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	pc->SetInputMode(InputMode);
}

void ARene_Lobby_GameMode::DisableUIControll()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	pc->SetInputMode(InputMode);
}


