// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Rene_Booth_GameState.h"
#include "Voice/Rene_VoiceChatManager.h"

ARene_Booth_GameState::ARene_Booth_GameState()
{
    // Create the VoiceChatManager component
    VoiceChatManager = CreateDefaultSubobject<URene_VoiceChatManager>(TEXT("VoiceChatManager"));
}

void ARene_Booth_GameState::AddPlayerState(APlayerState* PlayerState)
{
    Super::AddPlayerState(PlayerState);
}