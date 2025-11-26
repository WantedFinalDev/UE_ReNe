#include "Global/Rene_Booth_GameState.h"

#include "GameFramework/PlayerState.h"
#include "Global/Rebe_Booth_PlayerState.h"

void ARene_Booth_GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	
	if (ARebe_Booth_PlayerState* ps = Cast<ARebe_Booth_PlayerState>(PlayerState))
		ps->CreateUI();
}
