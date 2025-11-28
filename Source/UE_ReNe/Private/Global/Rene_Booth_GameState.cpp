#include "Global/Rene_Booth_GameState.h"

#include "GameFramework/PlayerState.h"
#include "Global/Rene_Booth_PlayerState.h"
#include "Player/Rene_PlayerController.h"

void ARene_Booth_GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	
	TObjectPtr<ARene_PlayerController> pc = Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(pc))
	{
		pc->OnPlayerListUpdated();
	}
}
