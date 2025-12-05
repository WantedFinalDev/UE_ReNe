#include "Global/Rene_Booth_GameState.h"
#include "Voice/Rene_VoiceChatManager.h"
#include "GameFramework/PlayerState.h"
#include "Global/Rene_PlayerState.h"
#include "Player/Rene_PlayerController.h"

ARene_Booth_GameState::ARene_Booth_GameState()
{
    // Create the VoiceChatManager component
    VoiceChatManager = CreateDefaultSubobject<URene_VoiceChatManager>(TEXT("VoiceChatManager"));
}

void ARene_Booth_GameState::AddPlayerState(APlayerState* PlayerState)
{
    Super::AddPlayerState(PlayerState);

    // 배열에만 추가, UI 업데이트는 실제로 리스트가 필요할 때 호출
    Rene_PlayerArray.AddUnique(Cast<ARene_PlayerState>(PlayerState));
}