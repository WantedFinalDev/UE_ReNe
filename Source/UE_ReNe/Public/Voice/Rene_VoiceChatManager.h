// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnlineSubsystem.h"
#include "Rene_VoiceChatManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_VoiceChatManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URene_VoiceChatManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	IOnlineSubsystem* Subsystem;
	IOnlineVoicePtr VoiceInterface;

	int32 LocalUserNum = 0;

public:
	UFUNCTION(BlueprintCallable)
	void StartVoice();

	UFUNCTION(BlueprintCallable)
	void StopVoice();

	UFUNCTION(NetMulticast, Reliable)
	void EstablishPrivateVoiceChannel(APlayerState* PlayerA, APlayerState* PlayerB);

	UFUNCTION(NetMulticast, Reliable)
	void EndPrivateVoiceChannel();
};
