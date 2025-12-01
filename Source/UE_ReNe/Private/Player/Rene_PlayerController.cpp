#include "Player/Rene_PlayerController.h"

#include "UE_ReNe.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Global/Rene_Booth_GameState.h"
#include "Widget/Rene_Company_Widget.h"
#include "Widget/Rene_Seeker_Widget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/VoiceInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogVoicePC, Log, All);

ARene_PlayerController::ARene_PlayerController()
{
	static ConstructorHelpers::FClassFinder<URene_Company_Widget> wbpcompany(TEXT("/Game/UI/WBP_Company_Widget.WBP_Company_Widget_C"));
	if (wbpcompany.Succeeded())
		companyui_class = wbpcompany.Class;
	
	static ConstructorHelpers::FClassFinder<URene_Seeker_Widget> wbpseeker(TEXT("/Game/UI/WBP_Seeker_Widget.WBP_Seeker_Widget_C"));
	if (wbpseeker.Succeeded())
		seekerui_class = wbpseeker.Class;
}

void ARene_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	EnsureVoiceInterface();

	
	SHOWWARN()
	
}

void ARene_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		// "VoicePushToTalk" 는 DefaultInput.ini 또는 ProjectSettings 에서 미리 등록되어 있어야 합니다.
		InputComponent->BindAction("VoicePushToTalk", IE_Pressed, this, &ARene_PlayerController::StartVoice);
		InputComponent->BindAction("VoicePushToTalk", IE_Released, this, &ARene_PlayerController::StopVoice);
	}
	else
	{
		UE_LOG(LogVoicePC, Warning, TEXT("SetupInputComponent: InputComponent is null"));
	}
}

void ARene_PlayerController::EnsureVoiceInterface()
{
	if (VoiceInterface.IsValid())
	{
		return;
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		UE_LOG(LogVoicePC, Warning, TEXT("EnsureVoiceInterface: OnlineSubsystem::Get() returned null"));
		return;
	}

	VoiceInterface = Subsystem->GetVoiceInterface();
	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogVoicePC, Warning, TEXT("EnsureVoiceInterface: GetVoiceInterface() returned null"));
	}
	else
	{
		UE_LOG(LogVoicePC, Log, TEXT("EnsureVoiceInterface: VoiceInterface acquired"));
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
	/*if (!IsValid(company_ui)) return;

	
	if (company_ui->IsVisible()) 
	{
		company_ui->PopulateUserList();
	}*/
	TObjectPtr<ARene_Booth_GameState> gs = GetWorld()->GetGameState<ARene_Booth_GameState>();
	if (IsValid(gs))
	{
		TArray<TObjectPtr<APlayerState>> allplayers = gs->PlayerArray;
		
		// TODO : Throw list to Company Widget
		/*
		 *	?? 여기 뭐였지
		 *	Populate에서 이미 구현중인 내용임.
		 */
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
	host->SetActorLocation(targetlocation + FVector(100, 100, 0));	// 캐릭터 겹침 방지 offset
	
	SHOWWARNF(TEXT("\nTeleport Complete | %s"), *targetstate->GetPlayerName())
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

void ARene_PlayerController::StartVoice()
{
	// 로컬 컨트롤러인지 확인
	if (!IsLocalController())
	{
		UE_LOG(LogVoicePC, Warning, TEXT("StartVoice called on non-local controller"));
		return;
	}

	EnsureVoiceInterface();
	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogVoicePC, Warning, TEXT("StartVoice: VoiceInterface invalid"));
		return;
	}

	int32 LocalUserNum = GetLocalUserNum();
	if (LocalUserNum < 0)
	{
		UE_LOG(LogVoicePC, Warning, TEXT("StartVoice: Invalid LocalUserNum (%d)"), LocalUserNum);
		return;
	}

	UE_LOG(LogVoicePC, Log, TEXT("StartVoice: LocalUserNum=%d - calling StartNetworkedVoice"), LocalUserNum);
	// StartNetworkedVoice는 로컬 오디오 캡처를 시작해 네트워크로 전송
	VoiceInterface->StartNetworkedVoice(LocalUserNum);
}

void ARene_PlayerController::StopVoice()
{    if (!IsLocalController())
{
	UE_LOG(LogVoicePC, Warning, TEXT("StopVoice called on non-local controller"));
	return;
}

	EnsureVoiceInterface();
	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogVoicePC, Warning, TEXT("StopVoice: VoiceInterface invalid"));
		return;
	}

	int32 LocalUserNum = GetLocalUserNum();
	if (LocalUserNum < 0)
	{
		UE_LOG(LogVoicePC, Warning, TEXT("StopVoice: Invalid LocalUserNum (%d)"), LocalUserNum);
		return;
	}

	UE_LOG(LogVoicePC, Log, TEXT("StopVoice: LocalUserNum=%d - calling StopNetworkedVoice"), LocalUserNum);
	VoiceInterface->StopNetworkedVoice(LocalUserNum);
}

int32 ARene_PlayerController::GetLocalUserNum() const
{    // 로컬 플레이어에서 ControllerId 가져오기 (PIE 및 로컬 실행에서 사용)
	if (IsLocalController())
	{
		ULocalPlayer* LP = GetLocalPlayer();
		if (LP)
		{
			return LP->GetControllerId();
		}
		else
		{
			// 로컬 플레이어가 없는 경우 기본 0 사용 (PIE 환경에서 보통 0)
			return 0;
		}
	}

	// 원격 컨트롤러인 경우 음성 시작 API를 로컬에서 호출하면 안됨 — 방어적 값
	return -1;
}
