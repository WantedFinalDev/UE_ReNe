#pragma once

#include "CoreMinimal.h"
#include "UE_ReNePlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/VoiceInterface.h"
#include "Voice/Rene_VoiceChatManager.h"
#include "Network/Rene_LocalVoiceRecorder.h" // New include for local voice recorder
#include "DesktopPlatformModule.h" // Required for file dialog
#include "Network/Rene_FileUploader.h" // Required for the new component
#include "Rene_PlayerController.generated.h"

class URene_LocalVoiceRecorder; // Forward declaration for the new component
class UInputAction;             // Forward declaration for Enhanced Input Action
class ARene_PlayerState; // Forward declaration for our PlayerState
class URene_InterviewWidget; // 전방 선언 추가

UCLASS()
class UE_RENE_API ARene_PlayerController : public AUE_ReNePlayerController
{
	GENERATED_BODY()
	
	/* Method */
public:
	ARene_PlayerController();
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateBoothUI();
	
	void CreateCompanyUI();
	void CreateSeekerUI();
	
	//	12.11 UI 통폐합으로 해당 기능은 삭제됨.
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_TeleportWithTarget(APlayerState* targetstate, FVector targetlocation);

	// New RPC to end the private interview
	UFUNCTION(Server, Reliable)
	void ServerRPC_EndInterview(APlayerState* InterviewerState, APlayerState* CandidateState);

	// 기존 텔레포트 RPC (AI 면접용)
	UFUNCTION(Server, Reliable)
	void ServerRPC_TeleportToLocation(FVector TargetLocation);

	// 새로 추가될 이동 및 착석 요청 RPC (Private 면접용)
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestMoveAndSit(FTransform TargetTransform);
	
	/*	InputAction, InputMappingContext 바인드 필요
	 *	@ key pressed -> On xx UI() 호출 : UI on/off
	 */
	UFUNCTION(BlueprintCallable)
	void OnCompanyUI();
	UFUNCTION(BlueprintCallable)
	void OnSeekerUI();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SendUserData(struct FReneUserData data);
    
	/**
	 *	윈도우 파일 탐색기 열기. (파일 업로드)
     * Opens a native file dialog for the player to select a file.
     * This is callable from Blueprints (e.g., the Lobby Widget).
     * @param DialogTitle The title to display on the file dialog window.
     * @param DefaultPath The default path to open the dialog to.
     * @param FileTypes A pipe-separated string of file types (e.g., "All Files (*.*)|*.*|PNG Files (*.png)|*.png").
     * @param OutFilePath The absolute path of the file the user selected.
     * @return True if the user selected a file, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "File Dialog")
    bool ShowFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath);


	void EnableUIControll();
	void DisableUIControll();
	
	// =================================================================
	//                 인터뷰 위젯 및 일어서기 관련 함수 (아래)
	// =================================================================
	
	// 캐릭터로부터 호출될 함수
	void ShowInterviewWidget();

	// 인터뷰 위젯으로부터 호출될 함수
	void EndInterview();

private:
	// '일어서기'를 서버에 요청하는 RPC
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestStandUp();

	// =================================================================
	
	
private:
	TObjectPtr<class UUserWidget> GetUserWidget();


	/* Field */
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Company_Widget> companyui_class;
	
	// GM : PostLogin에서 미리 생성함. 사라지지않음.
	UPROPERTY()
	TObjectPtr<URene_Company_Widget> company_ui;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Seeker_Widget> seekerui_class;
	
	// GM : PostLogin에서 미리 생성함. 사라지지않음.
	UPROPERTY()
	TObjectPtr<URene_Seeker_Widget> seeker_ui;

    /** The file uploader component for this player. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<URene_FileUploader> FileUploader;
	
//======voice======
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice")
	//URene_VoiceChatManager* VoiceChatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<URene_LocalVoiceRecorder> LocalVoiceRecorder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> PushToTalkAction;

	// Flag to indicate if the player is currently in an AI interview
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Interview")
	bool bIsInAIInterview;

	// The session ID for the current AI interview, received from the server.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Interview")
	FString AISessionID;

	/**
	 * Sets whether the player is currently in an AI interview.
	 * @param bNewState The new state for bIsInAIInterview.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Interview")
	void SetIsInAIInterview(bool bNewState);

	/**
	 * Sets the session ID for the current AI interview.
	 * @param NewSessionID The new session ID.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Interview")
	void SetAISessionID(const FString& NewSessionID);

	// =================================================================
	//                 인터뷰 위젯 관련 프로퍼티 (아래)
	// =================================================================

	// 블루프린트에서 인터뷰 위젯 클래스를 지정
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_InterviewWidget> InterviewWidgetClass;

private:
	// 생성된 인터뷰 위젯 인스턴스를 저장
	UPROPERTY()
	TObjectPtr<URene_InterviewWidget> InterviewWidgetInstance;

	// =================================================================

private:
	void OnStartTalking();
	void OnStopTalking();

};
