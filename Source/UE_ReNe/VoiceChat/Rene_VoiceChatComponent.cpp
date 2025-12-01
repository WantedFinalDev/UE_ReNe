// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceChat/Rene_VoiceChatComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
URene_VoiceChatComponent::URene_VoiceChatComponent()
{
	bIsVoiceAllowed = false;
}

void URene_VoiceChatComponent::StartTalking()
{
	if (bIsVoiceAllowed)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			PC->StartTalking();
		}
	}
}

void URene_VoiceChatComponent::StopTalking()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		PC->StopTalking();
	}
}

void URene_VoiceChatComponent::Client_EnterChatRoom_Implementation()
{
	bIsVoiceAllowed = true;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("System: You can now use Voice Chat (Hold 'V')"));
}

void URene_VoiceChatComponent::Client_ExitChatRoom_Implementation()
{
	bIsVoiceAllowed = false;
	StopTalking(); // Force stop talking when exiting
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("System: Voice Chat Disabled"));
}

bool URene_VoiceChatComponent::IsVoiceAllowed() const
{
	return bIsVoiceAllowed;
}
