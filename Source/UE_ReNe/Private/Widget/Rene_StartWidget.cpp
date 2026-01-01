#include "Widget/Rene_StartWidget.h"

#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Global/Rene_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/Rene_LobbyWidget.h"
#include "Widget/Rene_ProfileWidget.h"

void URene_StartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Enter->OnClicked.AddDynamic(this, &URene_StartWidget::OnClickedEnter);
	btn_Exit->OnClicked.AddDynamic(this, &URene_StartWidget::OnClickedExit);
	WBP_LobbyUI->WBP_ProfileUI->OnClickReturnDynamic.AddDynamic(this, &URene_StartWidget::OnClickedReturn);

	// Bind to GameInstance login delegate
	if (URene_GameInstance* gi = Cast<URene_GameInstance>(GetGameInstance()))
	{
		gi->OnLoginComplete.AddDynamic(this, &URene_StartWidget::OnLoginResponse);
	}
}

void URene_StartWidget::OnClickedEnter()
{
	if (etxt_ID->GetText().ToString() == TEXT("Test"))
	{
		TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(GetGameInstance());
		if (IsValidChecked(gi))
		{
			FString userRole = "jobseeker";
			if (cbox_Company->IsChecked())
			{
				userRole = "company";
			}
		
			// Disable button to prevent multiple clicks
			btn_Enter->SetIsEnabled(false);
			txt_Error->SetText(FText::FromString(TEXT("Logging in...")));

			// Request Login via GameInstance
			gi->RequestLogin(etxt_ID->GetText().ToString(), etxt_PW->GetText().ToString(), userRole);
		}
	}
	
	if (cbox_Company->IsChecked() && cbox_Seeker->IsChecked())
	{		
		txt_Error->SetText(FText::FromString(TEXT("Please select only one role.")));
		return;
	}
	if (!cbox_Company->IsChecked() && !cbox_Seeker->IsChecked()) 
	{
		txt_Error->SetText(FText::FromString(TEXT("Please select your role.")));
		return;
	}
	if (etxt_ID->GetText().IsEmpty())
	{
		txt_Error->SetText(FText::FromString(TEXT("Please enter your ID.")));
		return;
	}
	
	if (etxt_PW->GetText().IsEmpty())
	{
		txt_Error->SetText(FText::FromString(TEXT("Please enter your password.")));
		return;
	}

	TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(GetGameInstance());
	if (IsValidChecked(gi))
	{
		FString userRole = "jobseeker";
		if (cbox_Company->IsChecked())
		{
			userRole = "company";
		}
		
		// Disable button to prevent multiple clicks
		btn_Enter->SetIsEnabled(false);
		txt_Error->SetText(FText::FromString(TEXT("Logging in...")));

		// Request Login via GameInstance
		gi->RequestLogin(etxt_ID->GetText().ToString(), etxt_PW->GetText().ToString(), userRole);
	}
	else
	{
		LOGERROR()
	}
}

void URene_StartWidget::OnLoginResponse(bool bSuccess, const FReneUserData& UserData, FString ErrorMessage)
{
	// Re-enable button
	btn_Enter->SetIsEnabled(true);

	if (bSuccess)
	{
		// Login Successful
		txt_Error->SetText(FText::GetEmpty());
		sw_Switcher->SetActiveWidgetIndex(1);
	}
	else
	{
		// Login Failed
		txt_Error->SetText(FText::FromString(ErrorMessage));
	}
}

void URene_StartWidget::OnClickedExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void URene_StartWidget::OnClickedReturn()
{
	sw_Switcher->SetActiveWidgetIndex(0);
}


