#include "Global/Rene_Booth_GameMode.h"

#include "UE_ReNe.h"
#include "Global/Rene_Booth_PlayerState.h"
#include "Global/Rene_GameInstance.h"
#include "Player/Rene_PlayerController.h"

void ARene_Booth_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// TODO: New Player -> Player State -> Set Name
	/*
	 * Booth_GameState, AddPlayerState() 자동 호출
	 * Booth_GS로 가시오
	 */
	
	TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(NewPlayer->GetGameInstance());
	if (IsValid(gi))
	{
		FReneUserData data = gi->GetUserData();
		if (TObjectPtr<ARene_Booth_PlayerState> ps = Cast<ARene_Booth_PlayerState>(NewPlayer->PlayerState))
		{
			ps->SetPlayerName(data.Name);
			ps->SetReneUserData(data);
			SHOWWARNF(TEXT("User %s Saved"), *data.Name)
		}
	}
	
	
	if (ARene_PlayerController* pc = Cast<ARene_PlayerController>(NewPlayer))
	{
		if (pc->IsLocalPlayerController())
			pc->CreateCompanyUI();
		else
			pc->ClientRPC_CreateBoothUI();
	}
}
