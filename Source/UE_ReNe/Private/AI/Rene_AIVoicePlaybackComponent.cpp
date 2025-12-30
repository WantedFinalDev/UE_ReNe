// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Rene_AIVoicePlaybackComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Async/Async.h"
#include "Misc/Base64.h"
#include "Sound/SoundGroups.h"
#include "TimerManager.h" // Required for FTimerHandle and GetWorldTimerManager

URene_AIVoicePlaybackComponent::URene_AIVoicePlaybackComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // No need to tick for this component

	// Create and configure the AudioComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AIVoiceAudioComponent"));
	AudioComponent->SetupAttachment(this); // Attach to this component (now valid as this is a USceneComponent)
	AudioComponent->bAutoActivate = false; // Don't play automatically
	AudioComponent->SetIsReplicated(false); // Audio playback is client-side
	
	// Note: We are now using a manual timer for OnAudioFinished because USoundWaveProcedural's OnAudioFinished is unreliable.
	// AudioComponent->OnAudioFinished.AddDynamic(this, &URene_AIVoicePlaybackComponent::OnAudioFinished);
	
	// setting EnvelopeFollowerAttackTime and ReleaseTime, and binding the delegate with the correct signature.
	
	AudioComponent->EnvelopeFollowerAttackTime = 10;
	AudioComponent->EnvelopeFollowerReleaseTime = 10;
	
	AudioComponent->OnAudioSingleEnvelopeValue.AddDynamic(this, &URene_AIVoicePlaybackComponent::OnAudioEnvelopeValue);
}

void URene_AIVoicePlaybackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URene_AIVoicePlaybackComponent::OnAudioFinished()
{
	UE_LOG(LogTemp, Log, TEXT("AI Voice Playback Finished (Timer/Event)."));
	OnAIVoiceStateChanged.Broadcast(false);
	
	// Clear the timer if it's still active (safety)
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AudioFinishTimerHandle);
	}
}

// Fixed signature to match FOnAudioSingleEnvelopeValue: (const USoundWave* PlayingSoundWave, const float EnvelopeValue)
void URene_AIVoicePlaybackComponent::OnAudioEnvelopeValue(const USoundWave* PlayingSoundWave, const float EnvelopeValue)
{
	// Broadcast the current volume level to the Character
	OnAIVoiceAmplitudeChanged.Broadcast(EnvelopeValue);
}

bool URene_AIVoicePlaybackComponent::DecodeBase64(const FString& Base64String, TArray<uint8>& WavData)
{
	return FBase64::Decode(Base64String, WavData);
}

void URene_AIVoicePlaybackComponent::PlayAIVoiceFromBase64(const FString& Base64String)
{
	TArray<uint8> WavData;
	if (DecodeBase64(Base64String, WavData))
	{
		PlayAIVoiceFromWavData(WavData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to decode Base64 string for AI voice playback."));
	}
}

void URene_AIVoicePlaybackComponent::PlayAIVoiceFromWavData(const TArray<uint8>& WavData)
{
	if (WavData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to play empty WAV data."));
		return;
	}

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AudioComponent is null in URene_AIVoicePlaybackComponent."));
		return;
	}

	TArray<uint8> PCMData;
	int32 SampleRate = 0;
	int32 NumChannels = 0;

	if (!ParseWavData(WavData, PCMData, SampleRate, NumChannels))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse WAV data for AI voice playback."));
		return;
	}

	USoundWaveProcedural* SoundWaveProcedural = NewObject<USoundWaveProcedural>(this);
	if (SoundWaveProcedural)
	{
		SoundWaveProcedural->SetSampleRate(SampleRate);
		SoundWaveProcedural->NumChannels = NumChannels;
		
		// Calculate duration: TotalSamples / SampleRate
		// TotalSamples = PCMData.Num() / (NumChannels * BytesPerSample)
		// Assuming 16-bit PCM (2 bytes per sample)
		int32 BytesPerSample = 2;
		float Duration = (float)PCMData.Num() / (SampleRate * NumChannels * BytesPerSample);
		
		SoundWaveProcedural->Duration = Duration; 
		SoundWaveProcedural->SoundGroup = ESoundGroup::SOUNDGROUP_Effects;
		SoundWaveProcedural->bLooping = false;

		// Pass the PCM data to the procedural sound wave
		SoundWaveProcedural->QueueAudio(PCMData.GetData(), PCMData.Num());

		// Set the sound wave on the audio component and play
		AudioComponent->SetSound(SoundWaveProcedural);
		AudioComponent->Play();
		
		//재생 시작 이벤트 발생
		OnAIVoiceStateChanged.Broadcast(true);
		
		// Set a timer to manually trigger OnAudioFinished
		if (GetWorld())
		{
			// Add a small buffer (e.g., 0.1s) to ensure audio is fully done
			GetWorld()->GetTimerManager().SetTimer(AudioFinishTimerHandle, this, &URene_AIVoicePlaybackComponent::OnAudioFinished, Duration + 0.1f, false);
		}
		
		UE_LOG(LogTemp, Log, TEXT("Playing AI voice from WAV data. SampleRate: %d, Channels: %d, PCM Size: %d, Duration: %f"), SampleRate, NumChannels, PCMData.Num(), Duration);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create USoundWaveProcedural from WAV data."));
	}
}

bool URene_AIVoicePlaybackComponent::ParseWavData(const TArray<uint8>& WavData, TArray<uint8>& OutPCMData, int32& OutSampleRate, int32& OutNumChannels)
{
	// Basic WAV header parsing (assuming RIFF, WAVE, fmt, data chunks)
	// This is a simplified parser and might not handle all WAV formats.

	if (WavData.Num() < 44) // Minimum WAV header size
	{
		UE_LOG(LogTemp, Error, TEXT("WAV data too small to contain a valid header."));
		return false;
	}

	// Check RIFF header
	if (!(WavData[0] == 'R' && WavData[1] == 'I' && WavData[2] == 'F' && WavData[3] == 'F'))
	{
		UE_LOG(LogTemp, Error, TEXT("WAV Parse Error: RIFF header not found."));
		return false;
	}
	// Check WAVE header
	if (!(WavData[8] == 'W' && WavData[9] == 'A' && WavData[10] == 'V' && WavData[11] == 'E'))
	{
		UE_LOG(LogTemp, Error, TEXT("WAV Parse Error: WAVE header not found."));
		return false;
	}
	// Check fmt chunk
	if (!(WavData[12] == 'f' && WavData[13] == 'm' && WavData[14] == 't' && WavData[15] == ' '))
	{
		UE_LOG(LogTemp, Error, TEXT("WAV Parse Error: 'fmt ' chunk not found."));
		return false;
	}

	// Sample Rate (bytes 24-27)
	OutSampleRate = *reinterpret_cast<const int32*>(&(WavData[24]));
	// Num Channels (bytes 22-23)
	OutNumChannels = *reinterpret_cast<const int16*>(&(WavData[22]));
	// Bits Per Sample (bytes 34-35) - assuming 16-bit PCM for now
	int16 BitsPerSample = *reinterpret_cast<const int16*>(&(WavData[34]));

	if (BitsPerSample != 16)
	{
		UE_LOG(LogTemp, Warning, TEXT("Only 16-bit PCM WAV is fully supported by this parser. Found %d-bit."), BitsPerSample);
		// For now, we'll proceed, but this might cause issues if the audio format is not 16-bit PCM.
	}

	// Find 'data' chunk
	int32 DataChunkOffset = -1;
	for (int32 i = 36; i < WavData.Num() - 4; ++i)
	{
		if (WavData[i] == 'd' && WavData[i+1] == 'a' && WavData[i+2] == 't' && WavData[i+3] == 'a')
		{
			DataChunkOffset = i;
			break;
		}
	}

	if (DataChunkOffset == -1)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find 'data' chunk in WAV."));
		return false;
	}

	// Data size (bytes after 'data' tag)
	int32 DataSize = *reinterpret_cast<const int32*>(&(WavData[DataChunkOffset + 4]));

	if (DataChunkOffset + 8 + DataSize > WavData.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("WAV data size mismatch. Declared data size %d, but buffer ends earlier."), DataSize);
		return false;
	}

	OutPCMData.SetNumUninitialized(DataSize);
	FMemory::Memcpy(OutPCMData.GetData(), WavData.GetData() + DataChunkOffset + 8, DataSize);

	return true;
}
