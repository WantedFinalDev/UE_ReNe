#include "Player/Rene_PlayerController.h"

#include "UE_ReNe.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Global/Rene_Booth_GameState.h"
#include "Widget/Rene_Company_Widget.h"
#include "Widget/Rene_Seeker_Widget.h"

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
	
	SHOWWARN()
	
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
		
		// TODO : Transmission list to Company Widget
		/*
		 *
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
