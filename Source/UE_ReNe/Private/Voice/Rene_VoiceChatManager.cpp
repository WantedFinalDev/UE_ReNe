// Fill out your copyright notice in the Description page of Project Settings.


#include "Voice/Rene_VoiceChatManager.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/VoiceInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"


// Sets default values for this component's properties
URene_VoiceChatManager::URene_VoiceChatManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URene_VoiceChatManager::BeginPlay()
{
	Super::BeginPlay();

	Subsystem = Online::GetSubsystem(GetWorld());

	if (Subsystem)
	{
		VoiceInterface = Subsystem->GetVoiceInterface();

		if (VoiceInterface.IsValid())
		{
			// Start in a silent state by default
			VoiceInterface->StopNetworkedVoice(LocalUserNum);
			UE_LOG(LogTemp, Log, TEXT("Voice chat is initialized to OFF state by default."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Voice Interface is NOT valid. Check if Steam OSS supports voice."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Online Subsystem Not Found."));
	}
}


// Called every frame
void URene_VoiceChatManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URene_VoiceChatManager::StartVoice()
{
	if (VoiceInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("StartNetworkedVoice Called"));
		VoiceInterface->StartNetworkedVoice(LocalUserNum);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StartVoice failed - VoiceInterface invalid."));
	}
}

void URene_VoiceChatManager::StopVoice()
{
	if (VoiceInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("StopNetworkedVoice Called"));
		VoiceInterface->StopNetworkedVoice(LocalUserNum);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StopVoice failed - VoiceInterface invalid."));
	}
}

void URene_VoiceChatManager::EstablishPrivateVoiceChannel_Implementation(APlayerState* PlayerA, APlayerState* PlayerB)
{
    if (!VoiceInterface.IsValid()) return;

    APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
    if (!LocalPC || !LocalPC->PlayerState) return;

    const FUniqueNetIdRepl& LocalPlayerId = LocalPC->PlayerState->GetUniqueId();
    const FUniqueNetIdRepl& PlayerA_Id = PlayerA->GetUniqueId();
    const FUniqueNetIdRepl& PlayerB_Id = PlayerB->GetUniqueId();

    if (!LocalPlayerId.IsValid() || !PlayerA_Id.IsValid() || !PlayerB_Id.IsValid()) return;

    // --- Check if I am a participant in this private chat ---
    bool bIsParticipant = (*LocalPlayerId == *PlayerA_Id || *LocalPlayerId == *PlayerB_Id);

    if (bIsParticipant)
    {
        // 1. As a participant, find the partner and unmute them, while muting everyone else.
        // NOTE: The automatic call to StartNetworkedVoice has been removed to allow for Push-To-Talk control.

        // 2. Find the partner and unmute them, while muting everyone else.
        const FUniqueNetIdRepl& PartnerId = (*LocalPlayerId == *PlayerA_Id) ? PlayerB_Id : PlayerA_Id;
        
        for (APlayerState* OtherPlayer : GetWorld()->GetGameState()->PlayerArray)
        {
            if (OtherPlayer && OtherPlayer->GetUniqueId().IsValid() && *OtherPlayer->GetUniqueId() != *LocalPlayerId)
            {
                if (*OtherPlayer->GetUniqueId() == *PartnerId)
                {
                    // This is my partner, unmute them.
                    VoiceInterface->UnmuteRemoteTalker(LocalUserNum, *OtherPlayer->GetUniqueId(), false);
                }
                else
                {
                    // This is not my partner, mute them.
                    VoiceInterface->MuteRemoteTalker(LocalUserNum, *OtherPlayer->GetUniqueId(), false);
                }
            }
        }
    }
    else // I am an observer, not a participant.
    {
        // 1. Ensure my voice transmission is stopped.
        VoiceInterface->StopNetworkedVoice(LocalUserNum);
    }
}

void URene_VoiceChatManager::EndPrivateVoiceChannel_Implementation()
{
    if (VoiceInterface.IsValid())
    {
        // Regardless of role, every client stops transmitting voice.
        VoiceInterface->StopNetworkedVoice(LocalUserNum);
    }
}
