// Copyright Epic Games, Inc. All Rights Reserved.

#include "Global/Rene_Booth_GameMode.h"
#include "VoiceChat/Rene_VoiceSessionManager.h"
#include "UE_ReNe.h"
#include "Global/Rene_Booth_PlayerState.h"
#include "Global/Rene_GameInstance.h"
#include "Player/Rene_PlayerController.h"

ARene_Booth_GameMode::ARene_Booth_GameMode()
{
	VoiceSessionManager = CreateDefaultSubobject<URene_VoiceSessionManager>(TEXT("VoiceSessionManager"));
}

void ARene_Booth_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(NewPlayer->GetGameInstance());
	if (IsValid(gi))
	{
		FReneUserData data = gi->GetUserData();
		if (TObjectPtr<ARene_Booth_PlayerState> ps = Cast<ARene_Booth_PlayerState>(NewPlayer->PlayerState))
		{
			ps->SetPlayerName(data.Name);
			ps->SetReneUserData(data);
			SHOWWARNF(TEXT("User %s Saved"), *data.Name);
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

void ARene_Booth_GameMode::TriggerStart1v1Session()
{
	if (VoiceSessionManager)
	{
		VoiceSessionManager->Start1v1Session(GetWorld());
	}
}

void ARene_Booth_GameMode::TriggerEnd1v1Session()
{
	if (VoiceSessionManager)
	{
		VoiceSessionManager->End1v1Session(GetWorld());
	}
}