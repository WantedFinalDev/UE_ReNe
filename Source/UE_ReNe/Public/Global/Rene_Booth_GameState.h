#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Rene_Booth_GameState.generated.h"

class ARene_PlayerState;
class URene_VoiceChatManager;

UCLASS()
class UE_RENE_API ARene_Booth_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
	/* Method */
public:
    ARene_Booth_GameState(); // Add constructor declaration
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	
	
	
private:
	
	
	
	
	
	
	/* Field */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<URene_VoiceChatManager> VoiceChatManager;
	
	TArray<TObjectPtr<ARene_PlayerState>> Rene_PlayerArray;
	
	
	
	
private:
	
	
	
	
	
};
