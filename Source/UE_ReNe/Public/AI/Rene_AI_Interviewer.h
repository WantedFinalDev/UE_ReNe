// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Rene_AIVoicePlaybackComponent.h" // Include the new component header
#include "Rene_AI_Interviewer.generated.h"

UCLASS()
class UE_RENE_API ARene_AI_Interviewer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARene_AI_Interviewer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** The AI voice playback component for this interviewer. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Voice")
	TObjectPtr<URene_AIVoicePlaybackComponent> AIVoicePlaybackComponent;

	/**
	 * Plays the AI's voice response from a Base64 encoded WAV string.
	 * @param Base64String The Base64 encoded string of the WAV audio.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Voice")
	void PlayAIVoiceResponse(const FString& Base64String);
};
