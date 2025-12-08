#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rene_Booth_GameMode.generated.h"


UCLASS()
class UE_RENE_API ARene_Booth_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/**
	 * Starts a private 1-on-1 voice chat between two players.
	 * This should only be called on the server.
	 * @param PlayerA The first player in the conversation (e.g., the host).
	 * @param PlayerB The second player in the conversation (e.g., the selected client).
	 */
	void StartOneToOneVoiceChat(APlayerController* PlayerA, APlayerController* PlayerB);

	/**
	 * Ends a private 1-on-1 voice chat between two players.
	 * This should only be called on the server.
	 * @param PlayerA The first player in the conversation.
	 * @param PlayerB The second player in the conversation.
	 */
	void EndOneToOneVoiceChat(APlayerController* PlayerA, APlayerController* PlayerB);
	
	
private:
	
	
	
	
	
	/* Field */
public:
	
	
	
	
	
private:
	
	
	
	
	
};
