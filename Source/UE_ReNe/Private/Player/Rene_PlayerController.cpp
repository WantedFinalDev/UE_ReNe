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
	// Dedicated : User Info Check -> Co / Se
	//	Company/Sekker -> Host/Guest
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
	
	LOGWARNF(TEXT("Company UI has Gen"))
	company_ui = CreateWidget<URene_Company_Widget>(this, companyui_class);
	company_ui->AddToViewport();
	EnableUIControll();
}

void ARene_PlayerController::CreateSeekerUI()
{
	if (!IsValid(seekerui_class)) return;
	
	LOGWARNF(TEXT("Seeker UI has Gen"))
	seeker_ui = CreateWidget<URene_Seeker_Widget>(this, seekerui_class);
	seeker_ui->AddToViewport();
	EnableUIControll();
}

// 주석 처리 이유:
// - AddPlayerState에서 호출 시 PlayerState 이름이 아직 설정되기 전이라 EmptyName이 출력됨
// - PopulateUserList가 GameState의 Rene_PlayerArray를 직접 읽어서 UI 생성하므로 불필요
// - 필요 시 수동으로 호출 가능하도록 코드는 남겨둠
/*
void ARene_PlayerController::OnPlayerListUpdated()
{
	TObjectPtr<ARene_Booth_GameState> gs = GetWorld()->GetGameState<ARene_Booth_GameState>();
	if (IsValid(gs))
	{
		TArray<TObjectPtr<ARene_PlayerState>> allplayers = gs->Rene_PlayerArray;

		for (TObjectPtr<ARene_PlayerState> ps : allplayers)
		{
			LOGWARNF(TEXT("Player Name : %s"), *ps->GetReneUserName())
		}
	}
}
*/

//	12.11 UI 통폐합으로 텔레포트 기능 삭제됨.

/*void ARene_PlayerController::ServerRPC_TeleportWithTarget_Implementation(APlayerState* targetstate, FVector targetlocation)
{
	if (!HasAuthority()) return;
	APawn* host = GetPawn();
	APawn* target = targetstate ? targetstate->GetPawn() : nullptr;
	
	if (!host || !target) return;
	target->SetActorLocation(targetlocation);
	host->SetActorLocation(targetlocation + FVector(100, 100, 0));
	
	LOGWARNF(TEXT("\nTeleport Complete | %s"), *targetstate->GetPlayerName())

	// 기존 Voice Start Code는 이동되었음.
}*/

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
	LOGWARNF(TEXT("ServerRPC Start. Setting Name: %s"), *data.Name);

	ARene_PlayerState* ps = GetPlayerState<ARene_PlayerState>();
	if (ps)
	{
		ps->SetReneUserData(data);
		ps->SetPlayerName(data.Name);
		LOGWARNF(TEXT("ServerRPC Complete. PlayerName: %s"), *ps->GetReneUserName());
	}
	else
	{
		LOGERRORF(TEXT("ServerRPC Failed. PlayerName is EmptyName"));
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
	if (!IsLocalController())
	{
		UE_LOG(LogVoicePC, Warning, TEXT("OnStartTalking ignored: not local controller"));
		return;
	}

    // Get the current GameState and our custom PlayerState
    ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>();
    ARene_PlayerState* RenePlayerState = GetPlayerState<ARene_PlayerState>();

    // Only activate voice and recording if the player is in a private interview
    if (RenePlayerState && RenePlayerState->IsInPrivateInterview())
    {
        if (GameState && GameState->VoiceChatManager)
        {
            GameState->VoiceChatManager->StartVoice(); // Activate real-time P2P voice
        }

        if (LocalVoiceRecorder)
        {
            LocalVoiceRecorder->StartRecording(); // Activate local recording/sending
        }
    }
    // If not in a private interview, neither system will be activated by PTT.
}

void ARene_PlayerController::OnStopTalking()
{
	if (!IsLocalController())
	{
		UE_LOG(LogVoicePC, Warning, TEXT("OnStopTalking ignored: not local controller"));
		return;
	}

    // Get the VoiceChatManager from the GameState
    ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>();
    if (GameState && GameState->VoiceChatManager)
    {
        GameState->VoiceChatManager->StopVoice();
    }

    if (LocalVoiceRecorder)
    {
        LocalVoiceRecorder->StopAndUploadRecording();
    }
}

void ARene_PlayerController::ServerRPC_EndInterview_Implementation(APlayerState* InterviewerState, APlayerState* CandidateState)
{
    if (!HasAuthority())
    {
        return;
    }

    ARene_Booth_GameMode* GameMode = GetWorld()->GetAuthGameMode<ARene_Booth_GameMode>();
    if (GameMode)
    {
        APlayerController* InterviewerPC = InterviewerState ? Cast<APlayerController>(InterviewerState->GetOwner()) : nullptr;
        APlayerController* CandidatePC = CandidateState ? Cast<APlayerController>(CandidateState->GetOwner()) : nullptr;
        
        if (InterviewerPC && CandidatePC)
        {
            GameMode->EndOneToOneVoiceChat(InterviewerPC, CandidatePC);
        }
    }
}

void ARene_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Get the VoiceChatManager from the GameState to stop voice on EndPlay
    if (ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>())
    {
        if (GameState->VoiceChatManager)
        {
            GameState->VoiceChatManager->StopVoice();
        }
    }
	Super::EndPlay(EndPlayReason);
}

void ARene_PlayerController::ServerRPC_RequestMoveAndSit_Implementation(FTransform TargetTransform)
{
	if (!HasAuthority())
	{
		return;
	}

	// 캐스팅 대상을 AUE_ReNeCharacter로 변경
	if (AUE_ReNeCharacter* ControlledCharacter = Cast<AUE_ReNeCharacter>(GetPawn()))
	{
		ControlledCharacter->SetTargetSitTransform(TargetTransform);
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetTransform.GetLocation());
}

// 기존 텔레포트 RPC 구현 (변경 없음)
void ARene_PlayerController::ServerRPC_TeleportToLocation_Implementation(FVector TargetLocation)
{
	// 서버에서만 실행되도록 보장
	if (!HasAuthority())
	{
		return;
	}

	// 이 PlayerController가 조종하는 Pawn을 가져옵니다.
	if (APawn* ControlledPawn = GetPawn())
	{
		// Pawn의 위치를 클라이언트가 요청한 위치로 설정합니다.
		ControlledPawn->SetActorLocation(TargetLocation);
		LOGWARNF(TEXT("Teleporting pawn to %s"), *TargetLocation.ToString());
	}
}

// =================================================================
//                 인터뷰 위젯 및 일어서기 관련 함수 (아래)
// =================================================================

void ARene_PlayerController::ShowInterviewWidget()
{
	// 로컬 플레이어 컨트롤러에서만 UI를 생성합니다.
	if (!IsLocalController())
	{
		return;
	}

	// 위젯 클래스가 유효하고, 아직 위젯이 생성되지 않았다면
	if (InterviewWidgetClass && !InterviewWidgetInstance)
	{
		InterviewWidgetInstance = CreateWidget<URene_InterviewWidget>(this, InterviewWidgetClass);
		if (InterviewWidgetInstance)
		{
			InterviewWidgetInstance->AddToViewport();

			// 입력 모드를 UI 전용으로 변경하고 마우스 커서를 표시합니다.
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void ARene_PlayerController::EndInterview()
{
	// 로컬 플레이어 컨트롤러에서만 UI를 제거하고 입력 모드를 변경합니다.
	if (!IsLocalController())
	{
		return;
	}

	if (InterviewWidgetInstance)
	{
		InterviewWidgetInstance->RemoveFromParent();
		InterviewWidgetInstance = nullptr; // 포인터 정리

		// 입력 모드를 게임 전용으로 되돌리고 마우스 커서를 숨깁니다.
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}

	// 서버에 '일어서기'를 요청합니다.
	ServerRPC_RequestStandUp();
}

void ARene_PlayerController::ServerRPC_RequestStandUp_Implementation()
{
	if (AUE_ReNeCharacter* ControlledCharacter = Cast<AUE_ReNeCharacter>(GetPawn()))
	{
		ControlledCharacter->StandUp();
	}
}

// =================================================================
