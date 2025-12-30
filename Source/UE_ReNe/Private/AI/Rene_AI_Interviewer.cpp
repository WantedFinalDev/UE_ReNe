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
	
	// Bind to the amplitude update
	if (AIVoicePlaybackComponent)
	{
		AIVoicePlaybackComponent->OnAIVoiceAmplitudeChanged.AddDynamic(this, &ARene_AI_Interviewer::OnVoiceAmplitudeChanged);
	}
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

void ARene_AI_Interviewer::OnVoiceAmplitudeChanged(float Amplitude)
{
	// 1. Find the Face Mesh
	// MetaHumans usually have a skeletal mesh component named "Face"
	USkeletalMeshComponent* FaceMesh = nullptr;
	
	TArray<USkeletalMeshComponent*> SkeletalMeshes;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshes);
	
	for (USkeletalMeshComponent* SkeletalMesh : SkeletalMeshes)
	{
		if (SkeletalMesh->GetName().Contains(TEXT("Face")))
		{
			FaceMesh = SkeletalMesh;
			break;
		}
	}

	// 2. Apply Morph Targets
	if (FaceMesh)
	{
		// Amplitude is usually very small (0.0 to 0.2 for normal speech), so we multiply it.
		// You can tweak the '10.0f' multiplier to make the mouth open wider or less.
		float Sensitivity = 10.0f; 
		float JawValue = FMath::Clamp(Amplitude * Sensitivity, 0.0f, 1.0f);

		// "JawOpen" is the standard ARKit blendshape
		FaceMesh->SetMorphTarget(FName("JawOpen"), JawValue);
		
		// Optional: Add a little bit of "MouthFunnel" to make it look less like a nutcracker
		FaceMesh->SetMorphTarget(FName("MouthFunnel"), JawValue * 0.4f);
	}
}
