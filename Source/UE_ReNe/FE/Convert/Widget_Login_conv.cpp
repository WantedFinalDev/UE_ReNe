#include "Widget_Login_conv.h"

#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Global/Rene_GameInstance.h"
#include "Kismet/KismetSystemLibrary.h"


void UWidget_Login_conv::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (btn_Enter&&btn_Exit)
	{
		btn_Enter->OnClicked.AddDynamic(this, &UWidget_Login_conv::OnClickedEnter);
		btn_Exit->OnClicked.AddDynamic(this, &UWidget_Login_conv::OnClickedExit);
	}

	// Bind to GameInstance login delegate
	if (URene_GameInstance* gi = Cast<URene_GameInstance>(GetGameInstance()))
	{
		gi->OnLoginComplete.AddDynamic(this, &UWidget_Login_conv::OnLoginResponse);
	}
}

void UWidget_Login_conv::OnClickedEnter()
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
		return;
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

void UWidget_Login_conv::OnClickedExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UWidget_Login_conv::OnLoginResponse_Implementation(bool bSuccess, const FReneUserData& UserData, const FString& ErrorMessage)
{
	// Re-enable button
	btn_Enter->SetIsEnabled(true);

	if (bSuccess)
	{
		// Login Successful
		txt_Error->SetText(FText::GetEmpty());
		
		////*********////
		// sw_Switcher->SetActiveWidgetIndex(1);
		// Push Widget Stack 삽입 필요
	}
	else
	{
		// Login Failed
		txt_Error->SetText(FText::FromString(ErrorMessage));
	}	
}

