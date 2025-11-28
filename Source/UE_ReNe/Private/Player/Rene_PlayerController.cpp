#include "Player/Rene_PlayerController.h"

#include "UE_ReNe.h"
#include "Blueprint/UserWidget.h"
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
