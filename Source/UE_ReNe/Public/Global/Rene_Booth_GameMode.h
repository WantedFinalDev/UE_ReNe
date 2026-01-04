#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rene_Booth_GameMode.generated.h"


class ARene_PlayerController;

UCLASS()
class UE_RENE_API ARene_Booth_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// Determines the pawn class to spawn for a given player controller
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* Controller) override;

	/**
	 * Starts a private 1-on-1 voice chat between two players.
	 * This should only be called on the server.
	 * @param PlayerA The first player in the conversation (e.g., the host).
	 * @param PlayerB The second player in the conversation (e.g., the selected client).
	 */
	void StartOneToOneVoiceChat(APlayerController* PlayerA, APlayerController* PlayerB);
	void StartPublicVoiceChat(ARene_PlayerController* pc);

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
	// The pawn class to spawn for clients on specific maps
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<APawn> ClientPawnClass;
	
	
	
	
private:
	
	
	
	
	
};
