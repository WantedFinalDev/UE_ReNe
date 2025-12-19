#include "Player/Rene_PlayerController.h"

#include "UE_ReNe.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Global/Rene_Booth_GameState.h"
#include "Widget/Rene_Company_Widget.h"
#include "Widget/Rene_Seeker_Widget.h"
#include "Voice/Rene_VoiceChatManager.h"
#include "OnlineSubsystem.h"
#include "Interfaces/VoiceInterface.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h" // Required for Enhanced Input
#include "EnhancedInputComponent.h"   // Required for Enhanced Input
#include "Global/Rene_Booth_GameMode.h"
#include "Global/Rene_PlayerState.h"
#include "Global/Rene_GameInstance.h"
#include "Network/Rene_LocalVoiceRecorder.h"  // New include for local voice recorder
#include "UE_ReNeCharacter.h" // 헤더 변경
#include "AIController.h" // AIController 사용을 위해 헤더 추가
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI 기능 사용을 위한 핵심 헤더
#include "Widget/Rene_InterviewWidget.h" // 헤더 추가
#include "Widget/Rene_InterviewResultPopupWidget.h" // 헤더 추가
#include "Widget/Rene_WebViewWidget.h"


DEFINE_LOG_CATEGORY_STATIC(LogVoicePC, Log, All);

ARene_PlayerController::ARene_PlayerController()
{
	static ConstructorHelpers::FClassFinder<URene_Company_Widget> wbpcompany(TEXT("/Game/UI/WBP_Company_Widget.WBP_Company_Widget_C"));
	if (wbpcompany.Succeeded())
		companyui_class = wbpcompany.Class;
	
	static ConstructorHelpers::FClassFinder<URene_Seeker_Widget> wbpseeker(TEXT("/Game/UI/WBP_Seeker_Widget.WBP_Seeker_Widget_C"));
	if (wbpseeker.Succeeded())
		seekerui_class = wbpseeker.Class;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_menu(TEXT("/Game/Inputs/Actions/IA_Menu.IA_Menu"));
	if (tmp_ia_menu.Succeeded())
		ia_Menu = tmp_ia_menu.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Game/Inputs/IMC_Common.IMC_Common"));
	if (tmp_imc.Succeeded())
		imc_Common = tmp_imc.Object;
	
	LocalVoiceRecorder = CreateDefaultSubobject<URene_LocalVoiceRecorder>(TEXT("LocalVoiceRecorder")); // Create the new component
	FileUploader = CreateDefaultSubobject<URene_FileUploader>(TEXT("FileUploaderComponent"));

	bIsInAIInterview = false; // Initialize the flag
}

void ARene_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// EnhancedInput Mapping Context 추가
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (imc_Common)
			{
				Subsystem->AddMappingContext(imc_Common, 0);
			}
		}

		URene_GameInstance* gi = Cast<URene_GameInstance>(GetGameInstance());
		if (gi)
		{
			FReneUserData data = gi->GetCachedUserData();

			if (HasAuthority())
			{
				TObjectPtr<ARene_PlayerState> ps = GetPlayerState<ARene_PlayerState>();
				if (ps)
				{
					ps->SetReneUserData(data);
					ps->SetPlayerName(data.Name);
					LOGWARNF(TEXT("Host: PlayerState Set Directly - %s"), *data.Name);
				}
			}
			else
			{
				ServerRPC_SendUserData(data);
				LOGWARNF(TEXT("Client: ServerRPC Called - %s"), *data.Name);
			}
		}

		// Bind to the local voice recorder delegates
		if (LocalVoiceRecorder)
		{
			LocalVoiceRecorder->OnAIMessageReceived.AddDynamic(this, &ARene_PlayerController::OnAIMessageReceived);
			LocalVoiceRecorder->OnAIResponseStateChanged.AddDynamic(this, &ARene_PlayerController::OnAIResponseStateChanged);
			LocalVoiceRecorder->OnAIInterviewFinished.AddDynamic(this, &ARene_PlayerController::OnAIInterviewFinished);
		}
	}
}

void ARene_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
		{
			if (!PushToTalkAction)
			{
				LOGERRORF(TEXT("PushToTalkAction is not set in ARene_PlayerController. Push-to-talk will not work."));
				return;
			}
			if (!IsValid(ia_Menu))
			{
				LOGERRORF(TEXT("ia_Menu has Fucked"))
				return;
			}
			
			EIC->BindAction(PushToTalkAction, ETriggerEvent::Started, this, &ARene_PlayerController::OnStartTalking);
			EIC->BindAction(PushToTalkAction, ETriggerEvent::Completed, this, &ARene_PlayerController::OnStopTalking);
			EIC->BindAction(ia_Menu, ETriggerEvent::Triggered, this, &ARene_PlayerController::OnToggleMenu);
			
			
		}
		else
		{
			UE_LOG(LogVoicePC, Warning, TEXT("SetupInputComponent: EnhancedInputComponent is null."));
		}
	}
	else
	{
		UE_LOG(LogVoicePC, Warning, TEXT("SetupInputComponent: InputComponent is null."));
	}
}

bool ARene_PlayerController::ShowFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath)
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (DesktopPlatform)
    {
        TArray<FString> SelectedFiles;
        const bool bFileSelected = DesktopPlatform->OpenFileDialog(
            nullptr, // Parent window handle
            DialogTitle,
            DefaultPath,
            TEXT(""), // Default file
            FileTypes,
            EFileDialogFlags::None, // Single file selection
            SelectedFiles
        );

        if (bFileSelected && SelectedFiles.Num() > 0)
        {
            OutFilePath = SelectedFiles[0];
            return true;
        }
    }

    return false;
}

void ARene_PlayerController::ClientRPC_CreateBoothUI_Implementation()
{
	CreateSeekerUI();
}

void ARene_PlayerController::OnToggleMenu()
{
	if (HasAuthority())
	{
		if (!IsValid(company_ui))
		{
			LOGERRORF(TEXT("NULL Host UI"))
			return;
		}
		OnCompanyUI();
	}
	else
	{
		if (!IsValid(seeker_ui))
		{
			LOGERRORF(TEXT("NULL Client UI"))
			return;
		}
		OnSeekerUI();
	}
}

void ARene_PlayerController::CreateCompanyUI()
{
	if (!IsValid(companyui_class)) return;
	if (!IsLocalPlayerController() || !HasAuthority()) return;
	
	company_ui = CreateWidget<URene_Company_Widget>(this, companyui_class);
	company_ui->AddToViewport();
	EnableUIControll();
}

void ARene_PlayerController::CreateSeekerUI()
{
	if (!IsValid(seekerui_class)) return;
	
	seeker_ui = CreateWidget<URene_Seeker_Widget>(this, seekerui_class);
	seeker_ui->AddToViewport();
	EnableUIControll();
}

void ARene_PlayerController::OnCompanyUI()
{
	if (!HasAuthority() || !IsLocalPlayerController()) return;
	if (IsValid(company_ui))
	{
		company_ui->SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly im;
		SetInputMode(im);
		bShowMouseCursor = true;
	}
}

void ARene_PlayerController::OnSeekerUI()
{
	if (!IsLocalPlayerController() || HasAuthority()) return;
	if (IsValid(seeker_ui))
	{
		seeker_ui->SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly im;
		SetInputMode(im);
		bShowMouseCursor = true;
	}
}

void ARene_PlayerController::ServerRPC_SendUserData_Implementation(struct FReneUserData data)
{
	ARene_PlayerState* ps = GetPlayerState<ARene_PlayerState>();
	if (ps)
	{
		ps->SetReneUserData(data);
		ps->SetPlayerName(data.Name);
	}
}

void ARene_PlayerController::EnableUIControll()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI im;
	SetInputMode(im);
}

void ARene_PlayerController::DisableUIControll()
{
	bShowMouseCursor = false;
	FInputModeGameOnly im;
	SetInputMode(im);
}

TObjectPtr<class UUserWidget> ARene_PlayerController::GetUserWidget()
{
	if (IsValid(company_ui))
		return company_ui;
	else if (IsValid(seeker_ui))
		return seeker_ui;
	else
		return nullptr;
}

void ARene_PlayerController::OnStartTalking()
{
	if (!IsLocalController()) return;

    ARene_PlayerState* RenePlayerState = GetPlayerState<ARene_PlayerState>();

	if (bIsInAIInterview)
	{
		if (LocalVoiceRecorder) 
			LocalVoiceRecorder->StartRecording();
	}
    else if (RenePlayerState && RenePlayerState->IsInPrivateInterview())
    {
        if (ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>())
        {
            if (GameState->VoiceChatManager) 
            	GameState->VoiceChatManager->StartVoice();
        }
        if (LocalVoiceRecorder) 
        	LocalVoiceRecorder->StartRecording();
    }
}

void ARene_PlayerController::OnStopTalking()
{
	if (!IsLocalController()) return;

	if (bIsInAIInterview)
	{
		if (LocalVoiceRecorder) 
			LocalVoiceRecorder->StopAndUploadRecording(AISessionID);
	}
    else
    {
        if (ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>())
        {
            if (GameState->VoiceChatManager) 
            	GameState->VoiceChatManager->StopVoice();
        }
        if (LocalVoiceRecorder) 
        	LocalVoiceRecorder->StopAndUploadRecording(FString());
    }
}

void ARene_PlayerController::ServerRPC_EndInterview_Implementation(APlayerState* InterviewerState, APlayerState* CandidateState)
{
    if (!HasAuthority()) return;

	if (ARene_Booth_GameMode* GameMode = GetWorld()->GetAuthGameMode<ARene_Booth_GameMode>())
	{
		APlayerController* InterviewerPC =
			InterviewerState ? Cast<APlayerController>(InterviewerState->GetOwner()) : nullptr;

		APlayerController* CandidatePC =
			CandidateState ? Cast<APlayerController>(CandidateState->GetOwner()) : nullptr;

		if (InterviewerPC && CandidatePC)
		{
			GameMode->EndOneToOneVoiceChat(InterviewerPC, CandidatePC);
		}
	}
}

void ARene_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>())
    {
        if (GameState->VoiceChatManager) 
        	GameState->VoiceChatManager->StopVoice();
    }
	Super::EndPlay(EndPlayReason);
}

void ARene_PlayerController::ServerRPC_RequestMoveAndSit_Implementation(FTransform TargetTransform)
{
	if (!HasAuthority()) return;

	if (AUE_ReNeCharacter* ControlledCharacter = Cast<AUE_ReNeCharacter>(GetPawn()))
	{
		ControlledCharacter->SetTargetSitTransform(TargetTransform);
	}
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetTransform.GetLocation());
}

void ARene_PlayerController::ServerRPC_TeleportToLocation_Implementation(FVector TargetLocation)
{
	if (!HasAuthority()) return;

	if (APawn* ControlledPawn = GetPawn())
		ControlledPawn->SetActorLocation(TargetLocation);
}

void ARene_PlayerController::SetIsInAIInterview(bool bNewState)
{
	if (bIsInAIInterview == bNewState) return;

	bIsInAIInterview = bNewState;
	UE_LOG(LogVoicePC, Log, TEXT("SetIsInAIInterview: %s"), bNewState ? TEXT("True") : TEXT("False"));

	if (!bNewState)
	{
		if (APawn* PlayerPawn = GetPawn())
			SetViewTargetWithBlend(PlayerPawn, 0.0f);
	}
}

void ARene_PlayerController::SetAISessionID(const FString& NewSessionID)
{
	AISessionID = NewSessionID;
}

void ARene_PlayerController::ShowInterviewWidget()
{
	if (!IsLocalController()) return;

	if (InterviewWidgetClass && !InterviewWidgetInstance)
	{
		InterviewWidgetInstance = CreateWidget<URene_InterviewWidget>(this, InterviewWidgetClass);
		if (InterviewWidgetInstance)
		{
			InterviewWidgetInstance->AddToViewport();
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void ARene_PlayerController::EndInterview()
{
	if (!IsLocalController()) return;

	if (InterviewWidgetInstance)
	{
		InterviewWidgetInstance->RemoveFromParent();
		InterviewWidgetInstance = nullptr;
	}
	SetIsInAIInterview(false);
	ServerRPC_RequestStandUp();
	ServerRPC_TeleportToLocation(FVector(-559.999985,69.999981,112.000021));
}

void ARene_PlayerController::ServerRPC_RequestStandUp_Implementation()
{
	if (AUE_ReNeCharacter* ControlledCharacter = Cast<AUE_ReNeCharacter>(GetPawn()))
		ControlledCharacter->StandUp();
}

void ARene_PlayerController::OnAIMessageReceived(const FString& AIMessage)
{
	if (bIsInAIInterview && InterviewWidgetInstance)
		InterviewWidgetInstance->UpdateSubtitle(AIMessage);
}

void ARene_PlayerController::OnAIResponseStateChanged(bool bIsWaiting)
{
	if (bIsInAIInterview && InterviewWidgetInstance)
		InterviewWidgetInstance->SetLoadingState(bIsWaiting);
}

void ARene_PlayerController::DisplayInitialAIMessage(const FString& InitialMessage)
{
	if (bIsInAIInterview && InterviewWidgetInstance)
		InterviewWidgetInstance->UpdateSubtitle(InitialMessage);
}

void ARene_PlayerController::OnAIInterviewFinished(int32 InterviewResultID)
{
	UE_LOG(LogVoicePC, Log, TEXT("AI Interview Finished on Client. Received Result ID: %d. Requesting server to store it."), InterviewResultID);
	Server_SetInterviewResultID(InterviewResultID);
    ShowAIReportPage();
}

void ARene_PlayerController::Server_SetInterviewResultID_Implementation(int32 ResultID)
{
	ARene_PlayerState* RenePlayerState = GetPlayerState<ARene_PlayerState>();
	if (RenePlayerState)
	{
		RenePlayerState->SetInterviewResultID(ResultID);
		UE_LOG(LogVoicePC, Log, TEXT("Server stored InterviewResultID %d for player %s"), ResultID, *RenePlayerState->GetPlayerName());
	}
}

void ARene_PlayerController::ShowAIReportPage()
{
    if (!IsLocalController()) return;

    ARene_PlayerState* RenePlayerState = GetPlayerState<ARene_PlayerState>();
    if (!RenePlayerState) return;

    int32 ResultID = RenePlayerState->GetInterviewResultID();
    if (ResultID <= 0) return;

    if (InterviewResultPopupWidgetClass && !InterviewResultPopupInstance)
    {
        InterviewResultPopupInstance = CreateWidget<URene_InterviewResultPopupWidget>(this, InterviewResultPopupWidgetClass);
        if (InterviewResultPopupInstance)
        {
            InterviewResultPopupInstance->SetResultID(ResultID);
            InterviewResultPopupInstance->OnShowReportClicked.AddDynamic(this, &ARene_PlayerController::HandleShowReportClicked);
            InterviewResultPopupInstance->AddToViewport();
            
            FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;
        }
    }
}

void ARene_PlayerController::HandleShowReportClicked()
{
    if (WebViewWidgetClass && !WebViewInstance)
    {
        URene_GameInstance* GameInstance = GetGameInstance<URene_GameInstance>();
        if (!GameInstance) return;

        const FString ReportURL = GameInstance->GetNetworkSettings().AIReportURL;
        if (ReportURL.IsEmpty())
        {
            UE_LOG(LogVoicePC, Error, TEXT("AIReportURL is not set in NetworkSettings."));
            return;
        }

        WebViewInstance = CreateWidget<URene_WebViewWidget>(this, WebViewWidgetClass);
        if (WebViewInstance)
        {
            WebViewInstance->LoadURL(ReportURL);
            WebViewInstance->AddToViewport();
        }
    }
}

void ARene_PlayerController::CloseReportAndWebView()
{
    if (InterviewResultPopupInstance)
    {
        InterviewResultPopupInstance->RemoveFromParent();
        InterviewResultPopupInstance = nullptr;
    }
    if (WebViewInstance)
    {
        WebViewInstance->RemoveFromParent();
        WebViewInstance = nullptr;
    }
}
