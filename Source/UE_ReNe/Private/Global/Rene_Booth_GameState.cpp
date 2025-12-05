// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Rene_Booth_GameState.h"
#include "Voice/Rene_VoiceChatManager.h"
#include "GameFramework/PlayerState.h"
#include "Global/Rene_Booth_PlayerState.h"
#include "Player/Rene_PlayerController.h"

ARene_Booth_GameState::ARene_Booth_GameState()
{
    // Create the VoiceChatManager component
    VoiceChatManager = CreateDefaultSubobject<URene_VoiceChatManager>(TEXT("VoiceChatManager"));
}

void ARene_Booth_GameState::AddPlayerState(APlayerState* PlayerState)
{
    Super::AddPlayerState(PlayerState);

    TObjectPtr<ARene_PlayerController> pc = Cast<ARene_PlayerController>(GetWorld()->GetFirstPlayerController());
    if (IsValid(pc))
    {
        pc->OnPlayerListUpdated();
    }
}