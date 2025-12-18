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

class URene_LocalVoiceRecorder;
class UInputAction;
class ARene_PlayerState;
class URene_InterviewWidget;
class URene_InterviewResultPopupWidget;
class URene_WebViewWidget;

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
	
	void OnToggleMenu();
	
	void CreateCompanyUI();
	void CreateSeekerUI();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_EndInterview(APlayerState* InterviewerState, APlayerState* CandidateState);

	UFUNCTION(Server, Reliable)
	void ServerRPC_TeleportToLocation(FVector TargetLocation);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestMoveAndSit(FTransform TargetTransform);
	
	UFUNCTION(BlueprintCallable)
	void OnCompanyUI();
	UFUNCTION(BlueprintCallable)
	void OnSeekerUI();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SendUserData(struct FReneUserData data);
    
    UFUNCTION(BlueprintCallable, Category = "File Dialog")
    bool ShowFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath);

	UFUNCTION(BlueprintCallable)
	void EnableUIControll();
	UFUNCTION(BlueprintCallable)
	void DisableUIControll();
	
	void ShowInterviewWidget();
	void EndInterview();

	UFUNCTION(BlueprintCallable, Category = "AI Interview")
	void SetIsInAIInterview(bool bNewState);

	UFUNCTION(BlueprintCallable, Category = "AI Interview")
	void SetAISessionID(const FString& NewSessionID);

	UFUNCTION(BlueprintCallable, Category = "AI Interview")
	void DisplayInitialAIMessage(const FString& InitialMessage);

	/** Shows the AI interview report page in a web browser widget. */
	void ShowAIReportPage();

	/** Closes the report popup and the web view widget. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseReportAndWebView();

private:
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestStandUp();

	UFUNCTION(Server, Reliable)
	void Server_SetInterviewResultID(int32 ResultID);
	
	TObjectPtr<class UUserWidget> GetUserWidget();
	
	void OnStartTalking();
	void OnStopTalking();

protected:
	// AI Interview Event Handlers
	UFUNCTION()
	void OnAIMessageReceived(const FString& AIMessage);

	UFUNCTION()
	void OnAIResponseStateChanged(bool bIsWaiting);

	UFUNCTION()
	void OnAIInterviewFinished(int32 InterviewResultID);

	/** Handles the 'Show Report' button click from the result popup. */
	UFUNCTION()
	void HandleShowReportClicked();
	

	/* Field */
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Company_Widget> companyui_class;
	
	UPROPERTY()
	TObjectPtr<URene_Company_Widget> company_ui;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Seeker_Widget> seekerui_class;
	
	UPROPERTY()
	TObjectPtr<URene_Seeker_Widget> seeker_ui;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<URene_FileUploader> FileUploader;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<URene_LocalVoiceRecorder> LocalVoiceRecorder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> PushToTalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ia_Menu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> imc_Common;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Interview")
	bool bIsInAIInterview;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Interview")
	FString AISessionID;

	// 블루프린트에서 위젯 클래스를 지정
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_InterviewWidget> InterviewWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_InterviewResultPopupWidget> InterviewResultPopupWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URene_WebViewWidget> WebViewWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<URene_InterviewWidget> InterviewWidgetInstance;

	UPROPERTY()
	TObjectPtr<URene_InterviewResultPopupWidget> InterviewResultPopupInstance;

	UPROPERTY()
	TObjectPtr<URene_WebViewWidget> WebViewInstance;
};
