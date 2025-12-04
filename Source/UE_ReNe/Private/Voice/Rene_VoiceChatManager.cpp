// Fill out your copyright notice in the Description page of Project Settings.


#include "Voice/Rene_VoiceChatManager.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/VoiceInterface.h"



// Sets default values for this component's properties
URene_VoiceChatManager::URene_VoiceChatManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URene_VoiceChatManager::BeginPlay()
{
	Super::BeginPlay();

	Subsystem = Online::GetSubsystem(GetWorld());

	if (Subsystem)
	{
		VoiceInterface = Subsystem->GetVoiceInterface();

		if (!VoiceInterface.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Voice Interface is NOT valid. Check if Steam OSS supports voice."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Online Subsystem Not Found."));
	}	
}


// Called every frame
void URene_VoiceChatManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URene_VoiceChatManager::StartVoice()
{
	if (VoiceInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("StartNetworkedVoice Called"));
		VoiceInterface->StartNetworkedVoice(LocalUserNum);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StartVoice failed - VoiceInterface invalid."));
	}
}

void URene_VoiceChatManager::StopVoice()
{
	if (VoiceInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("StopNetworkedVoice Called"));
		VoiceInterface->StopNetworkedVoice(LocalUserNum);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StopVoice failed - VoiceInterface invalid."));
	}
}
