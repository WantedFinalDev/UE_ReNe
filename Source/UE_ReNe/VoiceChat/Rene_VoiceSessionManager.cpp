// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoiceChat/Rene_VoiceSessionManager.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "UE_ReNePlayerController.h"
#include "VoiceChat/Rene_VoiceChatComponent.h"
#include "GameFramework/Pawn.h"

void URene_VoiceSessionManager::Start1v1Session(UWorld* World)
{
	if (!World) return;

	const FVector RoomLocation(5000.f, 5000.f, 200.f);
	int PlayerCount = 0;
	AUE_ReNePlayerController* Player1 = nullptr;
	AUE_ReNePlayerController* Player2 = nullptr;

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		AUE_ReNePlayerController* PC = Cast<AUE_ReNePlayerController>(It->Get());
		if (PC)
		{
			if (PlayerCount == 0)
			{
				Player1 = PC;
			}
			else if (PlayerCount == 1)
			{
				Player2 = PC;
			}
			PlayerCount++;
		}
	}

	if (Player1 && Player1->VoiceChatComponent)
	{
		// if (APawn* HostPawn = Player1->GetPawn())
		// {
		// 	HostPawn->SetActorLocation(RoomLocation + FVector(-100, 0, 0));
		// }
		Player1->VoiceChatComponent->Client_EnterChatRoom();
	}

	if (Player2 && Player2->VoiceChatComponent)
	{
		// if (APawn* GuestPawn = Player2->GetPawn())
		// {
		// 	GuestPawn->SetActorLocation(RoomLocation + FVector(100, 0, 0));
		// }
		Player2->VoiceChatComponent->Client_EnterChatRoom();
	}
}

void URene_VoiceSessionManager::End1v1Session(UWorld* World)
{
	if (!World) return;

	const FVector LobbyLocation(0.f, 0.f, 200.f);

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		AUE_ReNePlayerController* PC = Cast<AUE_ReNePlayerController>(It->Get());
		if (PC && PC->VoiceChatComponent)
		{
			PC->VoiceChatComponent->Client_ExitChatRoom();
			// if (APawn* HostPawn = PC->GetPawn())
			// {
			// 	HostPawn->SetActorLocation(LobbyLocation);
			// }
		}
	}
}
