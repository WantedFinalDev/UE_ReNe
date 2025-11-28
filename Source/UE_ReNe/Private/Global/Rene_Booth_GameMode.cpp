#include "Global/Rene_Booth_GameMode.h"

#include "Player/Rene_PlayerController.h"

void ARene_Booth_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// TODO: NewPlayer -> PlayerState -> 이름 가져오기
	
	if (ARene_PlayerController* pc = Cast<ARene_PlayerController>(NewPlayer))
	{
		if (pc->IsLocalPlayerController())
			pc->CreateCompanyUI();
		else
			pc->ClientRPC_CreateBoothUI();
	}
}
