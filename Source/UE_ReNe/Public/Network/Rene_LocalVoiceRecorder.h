// Rene_LocalVoiceRecorder.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/VoiceCapture.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_LocalVoiceRecorder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUploadSuccess, const FString&, ResponseBody);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUploadFailure, const FString&, ErrorMessage);

// AI의 응답 메시지(자막)를 전달하기 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIMessageReceived, const FString&, AIMessage);
// "AI 생각 중" 상태 표시를 제어하기 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIResponseStateChanged, bool, bIsWaitingForResponse);
// AI 면접이 최종적으로 종료되고 결과 ID를 받았을 때 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIInterviewFinished, int32, InterviewResultID);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_LocalVoiceRecorder : public UActorComponent
{
    GENERATED_BODY()

public:
    URene_LocalVoiceRecorder();

    /** 업로드 성공 시 호출되는 델리게이트 */
    UPROPERTY(BlueprintAssignable, Category = "Voice Recorder")
    FOnUploadSuccess OnUploadSuccess;

    /** 업로드 실패 시 호출되는 델리게이트 */
    UPROPERTY(BlueprintAssignable, Category = "Voice Recorder")
    FOnUploadFailure OnUploadFailure;

    /** AI로부터 자막 메시지를 수신했을 때 호출됩니다. */
    UPROPERTY(BlueprintAssignable, Category = "AI Interview|Events")
    FOnAIMessageReceived OnAIMessageReceived;

    /** AI의 응답을 기다리는 상태가 변경될 때 호출됩니다. (true: 대기 시작, false: 대기 종료) */
    UPROPERTY(BlueprintAssignable, Category = "AI Interview|Events")
    FOnAIResponseStateChanged OnAIResponseStateChanged;

    /** AI 면접이 최종 종료되었을 때 호출됩니다. */
    UPROPERTY(BlueprintAssignable, Category = "AI Interview|Events")
    FOnAIInterviewFinished OnAIInterviewFinished;

    /** Starts capturing local microphone data. */
    void StartRecording();

    /**
     * Stops capturing and initiates the upload of the recorded data.
     * @param AISessionID The session ID for the AI interview, if applicable.
     */
    void StopAndUploadRecording(const FString& AISessionID);

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
     * @param AISessionID The session ID for the AI interview, if applicable.
     */
    void SendHttpRequest(const TArray<uint8>& VoiceData, const FString& PlayerName, const FString& AISessionID);

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
