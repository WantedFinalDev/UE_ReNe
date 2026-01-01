#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DesktopPlatformModule.h"
#include "UE_ReNePlayerController.h"
#include "Rene_PlayerController.generated.h"

class URene_Company_Widget;
class URene_Seeker_Widget;
class UInputMappingContext;
class UInputAction;
class URene_LocalVoiceRecorder;
class URene_FileUploader;
class URene_InterviewWidget;
class URene_InterviewResultPopupWidget;
class URene_WebViewWidget;

UCLASS()
class UE_RENE_API ARene_PlayerController : public AUE_ReNePlayerController
{
	GENERATED_BODY()

public:
	ARene_PlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "File")
	bool ShowFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath);

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateSeekerUI();
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateSeekerUI();

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateInfodeskUI();

	void OnToggleMenu();
	void CreateInfodeskUI();
	void OnCompanyUI();
	void OnSeekerUI();
	
	//	Widget Camera Moving
	UFUNCTION(BlueprintCallable)
	void SetWidgetCameraToInfo();
	UFUNCTION(BlueprintCallable)
	void SetWidgetCameraToMeet();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SendUserData(struct FReneUserData data);

	UFUNCTION(BlueprintCallable)
	void EnableUIControll();
	UFUNCTION(BlueprintCallable)
	void DisableUIControll();

	TObjectPtr<class UUserWidget> GetUserWidget();

	// --- Voice Chat ---
	void OnStartTalking();
	void OnStopTalking();

	UFUNCTION(Server, Reliable)
	void ServerRPC_EndInterview(APlayerState* InterviewerState, APlayerState* CandidateState);

	// --- Character Movement ---
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestMoveAndSit(FTransform TargetTransform);

	UFUNCTION(Server, Reliable)
	void ServerRPC_TeleportToLocation(FVector TargetLocation);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestStandUp();

	// --- AI Interview ---
	void SetIsInAIInterview(bool bNewState);
	void SetAISessionID(const FString& NewSessionID);
	void ShowInterviewWidget();
	void EndInterview();
	void DisplayInitialAIMessage(const FString& InitialMessage);
	void ShowAIReportPage();
	void HandleShowReportClicked();
	void CloseReportAndWebView();

	UFUNCTION(Server, Reliable)
	void Server_SetInterviewResultID(int32 ResultID);

	// --- AI Interview Event Handlers ---
	UFUNCTION()
	void OnAIMessageReceived(const FString& AIMessage);

	UFUNCTION()
	void OnAIResponseStateChanged(bool bIsWaiting);

	UFUNCTION()
	void OnAIInterviewFinished(int32 InterviewResultID);

	UFUNCTION()
	void OnAIVoiceStateChanged(bool bIsPlaying);

	UFUNCTION()
	void OnInterviewStageReceived(const FString& Stage);

	// --- P2P Interview Request Flow ---
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestPrivateInterview();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowInterviewRequest(const FString& RequestorName);

	UFUNCTION(Server, Reliable)
	void ServerRPC_AcceptPrivateInterview();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DeclinePrivateInterview();

	UFUNCTION(Client, Reliable)
	void ClientRPC_InterviewRequestDeclined();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URene_FileUploader> FileUploader;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URene_Company_Widget> companyui_class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URene_Seeker_Widget> seekerui_class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> wbp_infodesk;

	UPROPERTY()
	TObjectPtr<URene_Company_Widget> company_ui;

	UPROPERTY()
	TObjectPtr<URene_Seeker_Widget> seeker_ui;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> infodesk_ui;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> imc_Common;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ia_Menu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PushToTalkAction;

	// --- AI Interview ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URene_LocalVoiceRecorder> LocalVoiceRecorder;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_InterviewWidget> InterviewWidgetClass;

	UPROPERTY()
	TObjectPtr<URene_InterviewWidget> InterviewWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_InterviewResultPopupWidget> InterviewResultPopupWidgetClass;

	UPROPERTY()
	TObjectPtr<URene_InterviewResultPopupWidget> InterviewResultPopupInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_WebViewWidget> WebViewWidgetClass;

	UPROPERTY()
	TObjectPtr<URene_WebViewWidget> WebViewInstance;

	bool bIsInAIInterview;
	bool bIsAISpeaking;
	FString AISessionID;
	FString CurrentInterviewStage;

	// --- P2P Interview ---
	UPROPERTY()
	TObjectPtr<ARene_PlayerController> PendingRequestorPC;
};
