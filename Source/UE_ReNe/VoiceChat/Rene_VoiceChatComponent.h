// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rene_VoiceChatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RENE_API URene_VoiceChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URene_VoiceChatComponent();

	void StartTalking();
	void StopTalking();

	UFUNCTION(Client, Reliable)
	void Client_EnterChatRoom();

	UFUNCTION(Client, Reliable)
	void Client_ExitChatRoom();

	bool IsVoiceAllowed() const;

private:
	bool bIsVoiceAllowed;
	
};
