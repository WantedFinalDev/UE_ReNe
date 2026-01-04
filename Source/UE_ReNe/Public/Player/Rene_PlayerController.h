#pragma once

#include "CoreMinimal.h"
#include "UE_ReNePlayerController.h"
#include "DesktopPlatformModule.h"
#include "Rene_PlayerController.generated.h"

class URene_HUD;
class URene_Company_Widget;
class URene_Seeker_Widget;
class UInputMappingContext;
class UInputAction;
class URene_LocalVoiceRecorder;
class URene_FileUploader;
class URene_InterviewWidget;
class URene_InterviewResultPopupWidget;
class URene_WebViewWidget;
class URene_HostSitWidget;

UCLASS()
class UE_RENE_API ARene_PlayerController : public AUE_ReNePlayerController
{
	GENERATED_BODY()

	/* Method */
public:
	ARene_PlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable, Category = "File")
	bool ShowFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath);

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateSeekerUI();
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateSeekerUI();

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateInfodeskUI();
	
	UFUNCTION(BlueprintCallable)
	void OnToggleMenu();
	
	UFUNCTION(BlueprintCallable)
	void ShowInfodeskUI();
	
	UFUNCTION(BlueprintCallable)
	void ShowHUD();
	
	UFUNCTION()
	void OnToggleHomeMenu();
	
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
	
	UFUNCTION()
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
	void HandleInterviewResultIDUpdated(int32 NewResultID);

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

	// --- Host Movement ---
	void RequestMoveToHostSitTarget();

	
	
private:
	
	
	
	
	/* Field */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URene_FileUploader> FileUploader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URene_Company_Widget> companyui_class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URene_Seeker_Widget> seekerui_class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> info_widget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URene_HUD> HUD_Widget;
	
	UPROPERTY()
	TObjectPtr<URene_Company_Widget> company_ui;

	UPROPERTY()
	TObjectPtr<URene_Seeker_Widget> seeker_ui;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> infodesk_ui;
	
	UPROPERTY()
	TObjectPtr<URene_HUD> HUD_ui;
	
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

	// --- Host Sit ---
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_HostSitWidget> HostSitWidgetClass;

	TObjectPtr<AActor> HostSitTargetActor;

	bool bIsInAIInterview;
	bool bIsAISpeaking;
	FString AISessionID;
	FString CurrentInterviewStage;

	// --- P2P Interview ---
	UPROPERTY()
	TObjectPtr<ARene_PlayerController> PendingRequestorPC;

	bool bIsAutoMoving;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float HostSitWidgetShowDistance;

	// --- Auto Movement State ---
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsAutoMoving() const { return bIsAutoMoving; }

	// Public so the widget can null out its reference
	UPROPERTY()
	TObjectPtr<URene_HostSitWidget> HostSitWidgetInstance;

	void SetAutoMoving(bool bNewAutoMoving);
	
	
private:
	
	
	
	
};
