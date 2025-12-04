// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Rene_Booth_GameMode.h"
#include "Global/Rene_Booth_GameState.h"
#include "Voice/Rene_VoiceChatManager.h"
#include "GameFramework/PlayerState.h"

void ARene_Booth_GameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
}

void ARene_Booth_GameMode::StartOneToOneVoiceChat(APlayerController* PlayerA, APlayerController* PlayerB)
{
    // Ensure this logic only runs on the server.
    if (!HasAuthority())
    {
        return;
    }

    if (!PlayerA || !PlayerB)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartOneToOneVoiceChat failed: Player controllers are not valid."));
        return;
    }

    APlayerState* PlayerStateA = PlayerA->PlayerState;
    APlayerState* PlayerStateB = PlayerB->PlayerState;

    if (!PlayerStateA || !PlayerStateB)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartOneToOneVoiceChat failed: Player states are not valid."));
        return;
    }

    // Get the GameState, which owns the VoiceChatManager
    ARene_Booth_GameState* MyGameState = GetGameState<ARene_Booth_GameState>();
    if (!MyGameState)
    {
        UE_LOG(LogTemp, Error, TEXT("StartOneToOneVoiceChat failed: GameState not found."));
        return;
    }

    URene_VoiceChatManager* VoiceManager = MyGameState->VoiceChatManager;
    if (!VoiceManager)
    {
        UE_LOG(LogTemp, Error, TEXT("StartOneToOneVoiceChat failed: VoiceChatManager not found on GameState."));
        return;
    }

    // Call the multicast RPC to start the private chat for all clients
    VoiceManager->EstablishPrivateVoiceChannel(PlayerStateA, PlayerStateB);
}