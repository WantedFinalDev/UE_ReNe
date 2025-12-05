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
#include "EnhancedInputSubsystems.h" // Required for Enhanced Input
#include "EnhancedInputComponent.h"   // Required for Enhanced Input
#include "Global/Rene_Booth_GameMode.h"
#include "Network/Rene_LocalVoiceRecorder.h"  // New include for local voice recorder


DEFINE_LOG_CATEGORY_STATIC(LogVoicePC, Log, All);

ARene_PlayerController::ARene_PlayerController()
{
	static ConstructorHelpers::FClassFinder<URene_Company_Widget> wbpcompany(TEXT("/Game/UI/WBP_Company_Widget.WBP_Company_Widget_C"));
	if (wbpcompany.Succeeded())
		companyui_class = wbpcompany.Class;
	
	static ConstructorHelpers::FClassFinder<URene_Seeker_Widget> wbpseeker(TEXT("/Game/UI/WBP_Seeker_Widget.WBP_Seeker_Widget_C"));
	if (wbpseeker.Succeeded())
		seekerui_class = wbpseeker.Class;
	
	LocalVoiceRecorder = CreateDefaultSubobject<URene_LocalVoiceRecorder>(TEXT("LocalVoiceRecorder")); // Create the new component
}

void ARene_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SHOWWARN()
}

void ARene_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			if (PushToTalkAction)
			{
				EnhancedInputComponent->BindAction(PushToTalkAction, ETriggerEvent::Started, this, &ARene_PlayerController::OnStartTalking);
				EnhancedInputComponent->BindAction(PushToTalkAction, ETriggerEvent::Completed, this, &ARene_PlayerController::OnStopTalking);
			}
			else
			{
				UE_LOG(LogVoicePC, Warning, TEXT("PushToTalkAction is not set in ARene_PlayerController. Push-to-talk will not work."));
			}
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

void ARene_PlayerController::ClientRPC_CreateBoothUI_Implementation()
{
	// Dedic : User Info Check -> Co / Se
	CreateSeekerUI();
}

void ARene_PlayerController::CreateCompanyUI()
{
	if (!IsValid(companyui_class)) return;
	
	SHOWWARNF(TEXT("Company UI has Gen"))
	company_ui = CreateWidget<URene_Company_Widget>(this, companyui_class);
	company_ui->AddToViewport();
	EnableUIControll();
}

void ARene_PlayerController::CreateSeekerUI()
{
	if (!IsValid(seekerui_class)) return;
	
	SHOWWARNF(TEXT("Seeker UI has Gen"))
	seeker_ui = CreateWidget<URene_Seeker_Widget>(this, seekerui_class);
	seeker_ui->AddToViewport();
	EnableUIControll();
}

void ARene_PlayerController::OnPlayerListUpdated()
{
	TObjectPtr<ARene_Booth_GameState> gs = GetWorld()->GetGameState<ARene_Booth_GameState>();
	if (IsValid(gs))
	{
		TArray<TObjectPtr<APlayerState>> allplayers = gs->PlayerArray;
		
		for (TObjectPtr<APlayerState> ps : allplayers)
		{
			
			SHOWWARNF(TEXT("Player %s"), *ps->GetPlayerName())
		}
	}
}

TArray<TObjectPtr<class APlayerState>> ARene_PlayerController::GetAllPlayerState()
{
	TObjectPtr<ARene_Booth_GameState> gs = GetWorld()->GetGameState<ARene_Booth_GameState>();
	if (IsValid(gs))
	{
		return gs->PlayerArray;
	}
	return TArray<TObjectPtr<APlayerState>>();
}

void ARene_PlayerController::ServerRPC_TeleportWithTarget_Implementation(APlayerState* targetstate, FVector targetlocation)
{
	if (!HasAuthority()) return;
	APawn* host = GetPawn();
	APawn* target = targetstate ? targetstate->GetPawn() : nullptr;
	
	if (!host || !target) return;
	target->SetActorLocation(targetlocation);
	host->SetActorLocation(targetlocation + FVector(100, 100, 0));
	
	SHOWWARNF(TEXT("\nTeleport Complete | %s"), *targetstate->GetPlayerName())

	// +++ 여기에 1:1 보이스 채팅 시작 로직을 추가 +++
	ARene_Booth_GameMode* GameMode = GetWorld()->GetAuthGameMode<ARene_Booth_GameMode>();
	if (GameMode)
	{                                                                      
		// 이 RPC를 호출한 '나 자신(호스트)'의 PlayerController와
		// 텔레포트 대상인 'targetstate'의 PlayerController를 넘겨줍니다.       
		APlayerController* TargetPC = Cast<APlayerController>(targetstate->GetOwner());
		if (TargetPC)
		{                                                                   
			GameMode->StartOneToOneVoiceChat(this, TargetPC);                    
		}                                                                   
	}
}      

void ARene_PlayerController::OnCompanyUI()
{
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
	if (IsValid(seeker_ui))
	{
		seeker_ui->SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly im;
		SetInputMode(im);
		bShowMouseCursor = true;
	}
}

void ARene_PlayerController::EnableUIControll()
{
	bShowMouseCursor = true;
	FInputModeUIOnly im;
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

    // Get the VoiceChatManager from the GameState
    ARene_Booth_GameState* GameState = GetWorld()->GetGameState<ARene_Booth_GameState>();
    if (GameState && GameState->VoiceChatManager)
    {
        GameState->VoiceChatManager->StartVoice();
    }

    if (LocalVoiceRecorder)
    {
        LocalVoiceRecorder->StartRecording();
    }
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