#pragma once

#include "CoreMinimal.h"
#include "UE_ReNePlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Data/ReneUserData.h"
#include "Network/Rene_AIInterviewNetworkManager.h" // 헤더 추가
#include "Rene_PlayerController.generated.h"

class URene_Company_Widget;
class URene_Seeker_Widget;
class UInputMappingContext;
class UInputAction;
class URene_LocalVoiceRecorder;
class URene_FileUploader;
class URene_InterviewWidget;
class URene_InterviewResultPopupWidget;
class URene_HostSitWidget;
class URene_WebViewWidget;
class ARene_AI_Interviewer;

UCLASS()
class UE_RENE_API ARene_PlayerController : public AUE_ReNePlayerController
{
	GENERATED_BODY()
	
	/* Method */
public:
	ARene_PlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// File Dialog Helper
	UFUNCTION(BlueprintCallable, Category = "Rene|System")
	bool ShowFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath);

	// UI Management
	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateSeekerUI();
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateSeekerUI();
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateInfodeskUI();
	
	void OnToggleMenu();
	UFUNCTION(BlueprintCallable)
	void ShowInfodeskUI();
	UFUNCTION(BlueprintCallable)
	void ShowHUD();
	UFUNCTION(BlueprintCallable)
	void OnToggleHomeMenu();
	UFUNCTION(BlueprintCallable)
	void OnCompanyUI();
	UFUNCTION(BlueprintCallable)
	void OnSeekerUI();
	
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

	UFUNCTION(BlueprintCallable)
	class UUserWidget* GetUserWidget();

	// Voice Chat & Recording
	void OnStartTalking();
	void OnStopTalking();

	UFUNCTION(Server, Reliable)
	void ServerRPC_EndInterview(APlayerState* InterviewerState, APlayerState* CandidateState);

	// Movement & Interaction
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestMoveAndSit(FTransform TargetTransform);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_TeleportToLocation(FVector TargetLocation);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestStandUp();

	// AI Interview Logic
	void SetIsInAIInterview(bool bNewState);
	void SetAISessionID(const FString& NewSessionID);
	void ShowInterviewWidget();
	void EndInterview();

	UFUNCTION()
	void OnAIMessageReceived(const FString& AIMessage);
	UFUNCTION()
	void OnAIResponseStateChanged(bool bIsWaiting);
	UFUNCTION()
	void OnAIInterviewFinished(int32 InterviewResultID);
	UFUNCTION()
	void OnInterviewStageReceived(const FString& Stage);

	void DisplayInitialAIMessage(const FString& InitialMessage);

	UFUNCTION()
	void OnAIVoiceStateChanged(bool bIsPlaying);

	// AI Report Logic
	UFUNCTION()
	void HandleInterviewResultIDUpdated(int32 NewResultID);
	
	UFUNCTION(Server, Reliable)
	void Server_SetInterviewResultID(int32 ResultID);

	void ShowAIReportPage();

	UFUNCTION()
	void HandleShowReportClicked();

	void CloseReportAndWebView();

	// P2P Interview Request Logic
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

	// Host Sit Logic
	void RequestMoveToHostSitTarget();
	void SetAutoMoving(bool bNewAutoMoving);
	UFUNCTION(BlueprintPure)
	bool IsAutoMoving() const { return bIsAutoMoving; }

	// [추가] AI 면접 시작 요청 (위젯에서 호출)
	void RequestAIInterviewStart(const FString& URL, int32 UserID, int32 CompanyID, int32 JobGroupID, AActor* TargetActor, ARene_AI_Interviewer* Interviewer);

	
	/* Field */
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> companyui_class;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> seekerui_class;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> info_widget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HUD_Widget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> InterviewWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> InterviewResultPopupWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HostSitWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WebViewWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> imc_Common;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> ia_Menu;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PushToTalkAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<URene_LocalVoiceRecorder> LocalVoiceRecorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Network")
	TObjectPtr<URene_FileUploader> FileUploader;

	// [추가] AI 면접 네트워크 매니저
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Network")
	TObjectPtr<URene_AIInterviewNetworkManager> AIInterviewManager;

	// [이동] private -> public (Rene_HostSitWidget에서 접근 필요)
	UPROPERTY()
	TObjectPtr<URene_HostSitWidget> HostSitWidgetInstance;

private:
	UPROPERTY()
	TObjectPtr<class URene_Company_Widget> company_ui;
	UPROPERTY()
	TObjectPtr<class URene_Seeker_Widget> seeker_ui;
	UPROPERTY()
	TObjectPtr<class UUserWidget> infodesk_ui;
	UPROPERTY()
	TObjectPtr<class URene_HUD> HUD_ui;
	UPROPERTY()
	TObjectPtr<URene_InterviewWidget> InterviewWidgetInstance;
	UPROPERTY()
	TObjectPtr<URene_InterviewResultPopupWidget> InterviewResultPopupInstance;
	
	UPROPERTY()
	TObjectPtr<URene_WebViewWidget> WebViewInstance;

	bool bIsInAIInterview;
	FString AISessionID;
	bool bIsAISpeaking;
	FString CurrentInterviewStage;

	// [추가] 위젯 생성 전 수신된 초기 AI 메시지 캐싱
	FString CachedInitialAIMessage;

	// P2P Interview Request Variables
	UPROPERTY()
	TObjectPtr<ARene_PlayerController> PendingRequestorPC;

	// Host Sit Variables
	UPROPERTY()
	TObjectPtr<AActor> HostSitTargetActor;
	
	float HostSitWidgetShowDistance;
	bool bIsAutoMoving;
};
