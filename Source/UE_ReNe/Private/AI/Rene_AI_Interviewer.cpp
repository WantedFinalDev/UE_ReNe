// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Rene_AI_Interviewer.h"
#include "AI/Rene_AIVoicePlaybackComponent.h" // Include the new component header


// Sets default values
ARene_AI_Interviewer::ARene_AI_Interviewer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the AI Voice Playback Component
	AIVoicePlaybackComponent = CreateDefaultSubobject<URene_AIVoicePlaybackComponent>(TEXT("AIVoicePlaybackComponent"));
	// Attach the component to the root component of the character
	AIVoicePlaybackComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ARene_AI_Interviewer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARene_AI_Interviewer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARene_AI_Interviewer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARene_AI_Interviewer::PlayAIVoiceResponse(const FString& Base64String)
{
	if (AIVoicePlaybackComponent)
	{
		AIVoicePlaybackComponent->PlayAIVoiceFromBase64(Base64String);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AIVoicePlaybackComponent is null on ARene_AI_Interviewer."));
	}
}
