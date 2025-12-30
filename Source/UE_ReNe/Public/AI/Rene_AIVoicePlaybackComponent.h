// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Rene_AIVoicePlaybackComponent.generated.h"

// "AI 음성 재생 중" 상태 표시를 제어하기 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIVoiceStateChanged, bool, bIsPlaying);

// Delegate to broadcast amplitude (0.0 to 1.0)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIVoiceAmplitudeChanged, float, Amplitude);

class UAudioComponent;
class USoundWaveProcedural; // Forward declaration for USoundWaveProcedural
class USoundWave; // Forward declaration for USoundWave

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_AIVoicePlaybackComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	URene_AIVoicePlaybackComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "AI Voice")
	FOnAIVoiceStateChanged OnAIVoiceStateChanged;

	// The event that the Character will listen to
	UPROPERTY(BlueprintAssignable, Category = "AI Voice")
	FOnAIVoiceAmplitudeChanged OnAIVoiceAmplitudeChanged;
	
	UFUNCTION()
	void OnAudioFinished();

	// Internal callback from the Audio Engine
	// Corrected signature to match FOnAudioSingleEnvelopeValue
	UFUNCTION()
	void OnAudioEnvelopeValue(const class USoundWave* PlayingSoundWave, const float EnvelopeValue);
	
	/**
	 * Decodes a Base64 encoded string into a TArray of bytes.
	 * @param Base64String The Base64 string to decode.
	 * @param WavData The output array of bytes.
	 * @return True if decoding was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Voice")
	bool DecodeBase64(const FString& Base64String, TArray<uint8>& WavData);

	/**
	 * Plays audio from a Base64 encoded WAV file string.
	 * @param Base64String The Base64 encoded string representing the WAV file.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Voice")
	void PlayAIVoiceFromBase64(const FString& Base64String);

	/**
	 * Plays audio from raw WAV data.
	 * @param WavData The raw byte array of the WAV file.
	 */
	void PlayAIVoiceFromWavData(const TArray<uint8>& WavData);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	// Timer handle for manual audio finish event
	FTimerHandle AudioFinishTimerHandle;

	// Helper function to parse WAV header and extract PCM data
	bool ParseWavData(const TArray<uint8>& WavData, TArray<uint8>& OutPCMData, int32& OutSampleRate, int32& OutNumChannels);
};
