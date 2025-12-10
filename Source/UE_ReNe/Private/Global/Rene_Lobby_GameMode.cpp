#include "Global/Rene_Lobby_GameMode.h"
#include "UE_ReNe.h"
#include "UE_ReNePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Player/Rene_PlayerController.h"
#include "Widget/Rene_LobbyWidget.h"

/*void ARene_Lobby_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	/*if(!IsValid(LobbyUIClass))
		LOGERRORF(TEXT("LobbyUIClass must be set in BP_Lobby_GameMode!"));#1#
	
}*/

/*void ARene_Lobby_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	DisplayLobbyUI(NewPlayer);
}

void ARene_Lobby_GameMode::DisplayLobbyUI(APlayerController* pc)
{
	LobbyUI = CreateWidget<URene_LobbyWidget>(pc, LobbyUIClass);
	LobbyUI->AddToViewport();
	Cast<ARene_PlayerController>(pc)->EnableUIControll();
	//
}

void ARene_Lobby_GameMode::SwitchDisplayUI()
{
	LOGWARNF(TEXT("Visibility : %s"), *UEnum::GetValueAsString(LobbyUI->GetVisibility()))
	if (!IsValid(LobbyUI)) return;
	if(LobbyUI->GetVisibility() != ESlateVisibility::Collapsed)
	{
		LOGWARN()
		LobbyUI->SetVisibility(ESlateVisibility::Collapsed);
		Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController())->DisableUIControll();
	}
	else
	{
		LOGWARN()
		LobbyUI->SetVisibility(ESlateVisibility::Visible);
		Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController())->EnableUIControll();
	}

}*/




