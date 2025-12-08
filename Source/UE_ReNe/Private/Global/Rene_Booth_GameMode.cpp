#include "Global/Rene_Booth_GameMode.h"
#include "UE_ReNe.h"
#include "Global/Rene_PlayerState.h"
#include "Global/Rene_GameInstance.h"
#include "Player/Rene_PlayerController.h"
#include "Global/Rene_Booth_GameMode.h"
#include "Global/Rene_Booth_GameState.h"
#include "Voice/Rene_VoiceChatManager.h"
#include "GameFramework/PlayerState.h"

void ARene_Booth_GameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
	
    // TODO: New Player -> Player State -> Set Name
    /*
     * Booth_GameState, AddPlayerState() 자동 호출
     * Booth_GS로 가시오
     */
	
    /*TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(NewPlayer->GetGameInstance());
    if (IsValid(gi))
    {
        FReneUserData data = gi->GetUserData();
        if (TObjectPtr<ARene_PlayerState> ps = Cast<ARene_PlayerState>(NewPlayer->PlayerState))
        {
            ps->SetPlayerName(data.Name);
            ps->SetReneUserData(data);
            LOGWARNF(TEXT("User %s Saved"), *data.Name)
        }
    }*/
	
    if (ARene_PlayerController* pc = Cast<ARene_PlayerController>(NewPlayer))
    {
        if (pc->IsLocalPlayerController())
            pc->CreateCompanyUI();
        else
            pc->ClientRPC_CreateBoothUI();
    }
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
        LOGWARNF(TEXT("StartOneToOneVoiceChat failed: Player controllers are not valid."));
        return;
        
    }

    APlayerState* PlayerStateA_Base = PlayerA->PlayerState;
    APlayerState* PlayerStateB_Base = PlayerB->PlayerState;

    if (!PlayerStateA_Base || !PlayerStateB_Base)
    {
        LOGERRORF(TEXT("StartOneToOneVoiceChat failed: Player states are not valid."));
        return;
    }

    ARene_Booth_PlayerState* PlayerStateA = Cast<ARene_Booth_PlayerState>(PlayerStateA_Base);
    ARene_Booth_PlayerState* PlayerStateB = Cast<ARene_Booth_PlayerState>(PlayerStateB_Base);

    if (!PlayerStateA || !PlayerStateB)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartOneToOneVoiceChat failed: Custom Player states are not valid."));
        return;
    }

    // Set the bIsInPrivateInterview flag for both participants
    PlayerStateA->bIsInPrivateInterview = true;
    PlayerStateB->bIsInPrivateInterview = true;

    // Get the GameState, which owns the VoiceChatManager
    ARene_Booth_GameState* MyGameState = GetGameState<ARene_Booth_GameState>();
    if (!MyGameState)
    {
        LOGERRORF(TEXT("StartOneToOneVoiceChat failed: GameState not found."));
        return;
    }

    URene_VoiceChatManager* VoiceManager = MyGameState->VoiceChatManager;
    if (!VoiceManager)
    {
        LOGERRORF(TEXT("StartOneToOneVoiceChat failed: VoiceChatManager not found on GameState."));
        return;
    }

    // Call the multicast RPC to start the private chat for all clients
    VoiceManager->EstablishPrivateVoiceChannel(PlayerStateA, PlayerStateB);
}

void ARene_Booth_GameMode::EndOneToOneVoiceChat(APlayerController* PlayerA, APlayerController* PlayerB)
{
    // Ensure this logic only runs on the server.
    if (!HasAuthority())
    {
        return;
    }

    if (!PlayerA || !PlayerB)
    {
        UE_LOG(LogTemp, Warning, TEXT("EndOneToOneVoiceChat failed: Player controllers are not valid."));
        return;
    }

    APlayerState* PlayerStateA_Base = PlayerA->PlayerState;
    APlayerState* PlayerStateB_Base = PlayerB->PlayerState;

    if (!PlayerStateA_Base || !PlayerStateB_Base)
    {
        UE_LOG(LogTemp, Warning, TEXT("EndOneToOneVoiceChat failed: Player states are not valid."));
        return;
    }

    ARene_Booth_PlayerState* PlayerStateA = Cast<ARene_Booth_PlayerState>(PlayerStateA_Base);
    ARene_Booth_PlayerState* PlayerStateB = Cast<ARene_Booth_PlayerState>(PlayerStateB_Base);

    if (!PlayerStateA || !PlayerStateB)
    {
        UE_LOG(LogTemp, Warning, TEXT("EndOneToOneVoiceChat failed: Custom Player states are not valid."));
        return;
    }

    // Reset the bIsInPrivateInterview flag for both participants
    PlayerStateA->bIsInPrivateInterview = false;
    PlayerStateB->bIsInPrivateInterview = false;

    // Get the GameState, which owns the VoiceChatManager
    ARene_Booth_GameState* MyGameState = GetGameState<ARene_Booth_GameState>();
    if (!MyGameState)
    {
        UE_LOG(LogTemp, Error, TEXT("EndOneToOneVoiceChat failed: GameState not found."));
        return;
    }

    URene_VoiceChatManager* VoiceManager = MyGameState->VoiceChatManager;
    if (!VoiceManager)
    {
        UE_LOG(LogTemp, Error, TEXT("EndOneToOneVoiceChat failed: VoiceChatManager not found on GameState."));
        return;
    }

    // Call the multicast RPC to end the private chat for all clients
    VoiceManager->EndPrivateVoiceChannel();
}