// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rene_Booth_GameMode.generated.h"

class URene_VoiceSessionManager;

/**
 * 
 */
UCLASS()
class UE_RENE_API ARene_Booth_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARene_Booth_GameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	void TriggerStart1v1Session();
	void TriggerEnd1v1Session();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice Chat")
	URene_VoiceSessionManager* VoiceSessionManager;
	
};