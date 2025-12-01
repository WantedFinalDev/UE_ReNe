// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Rene_VoiceSessionManager.generated.h"

/**
 * Manages the 1v1 voice chat session logic.
 */
UCLASS()
class UE_RENE_API URene_VoiceSessionManager : public UObject
{
	GENERATED_BODY()

public:
	void Start1v1Session(UWorld* World);
	void End1v1Session(UWorld* World);
};
