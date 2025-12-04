// Rene_LocalVoiceRecorder.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/VoiceCapture.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_LocalVoiceRecorder.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_LocalVoiceRecorder : public UActorComponent
{
    GENERATED_BODY()

public:
    URene_LocalVoiceRecorder();

    // The URL of your server endpoint that accepts the voice data.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP")
    FString HttpUploadURL = TEXT("http://100.115.252.51:8000/api/v1/rene/begin/voice-chat");

    /** Starts capturing local microphone data. */
    void StartRecording();

    /** Stops capturing and initiates the upload of the recorded data. */
    void StopAndUploadRecording();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    /** Initializes the voice capture device. */
    void InitializeVoiceCapture();

    /** Called by a timer to pull PCM data from the voice capture device. */
    void CaptureVoiceData();

    /**
     * Sends the recorded voice data as a multipart/form-data POST request.
     * @param VoiceData The raw PCM data to upload.
     * @param PlayerName The name of the player who recorded the audio.
     */
    void SendHttpRequest(const TArray<uint8>& VoiceData, const FString& PlayerName);

    /**
     * Callback function for when the HTTP upload request completes.
     * @param Request The original request object.
     * @param Response The response object from the server.
     * @param bWasSuccessful True if the request was successfully processed.
     */
    void OnUploadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    // The UE Voice Capture interface.
    TSharedPtr<IVoiceCapture> VoiceCapture;

    // Buffer to accumulate raw PCM voice data.
    TArray<uint8> RecordedVoiceData;

    // Timer for polling the voice capture buffer.
    FTimerHandle VoiceCaptureTimerHandle;

    // Flag to indicate if we are currently capturing audio.
    bool bIsRecording;

    // Thread-safe access to the voice data buffer.
    FCriticalSection VoiceBufferCriticalSection;
};
