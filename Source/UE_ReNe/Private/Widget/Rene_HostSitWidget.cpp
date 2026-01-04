// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Rene_HostSitWidget.h"
#include "Components/Button.h"
#include "Player/Rene_PlayerController.h"
#include "UE_ReNe.h"

void URene_HostSitWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_SitHost)
	{
		btn_SitHost->OnClicked.AddDynamic(this, &URene_HostSitWidget::OnSitHostClicked);
	}

	if (btn_Back)
	{
		btn_Back->OnClicked.AddDynamic(this, &URene_HostSitWidget::OnBackClicked);
	}
}

void URene_HostSitWidget::OnSitHostClicked()
{
	ARene_PlayerController* PC = Cast<ARene_PlayerController>(GetOwningPlayer());
	if (PC)
	{
		UE_LOG(Rene, Log, TEXT("URene_HostSitWidget: Sit button clicked. Requesting move."));
		PC->RequestMoveToHostSitTarget();
		PC->DisableUIControll(); // UI 모드 비활성화
        
        // Also null out the instance in the player controller so it can be re-created
        PC->HostSitWidgetInstance = nullptr;
	}
    else
    {
        UE_LOG(Rene, Warning, TEXT("URene_HostSitWidget: Could not get PlayerController."));
    }
    
    // Hide widget after clicking
    RemoveFromParent();
}

void URene_HostSitWidget::OnBackClicked()
{
	ARene_PlayerController* PC = Cast<ARene_PlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->DisableUIControll();
		// Also null out the instance in the player controller so it can be re-created
		PC->HostSitWidgetInstance = nullptr;
	}

	RemoveFromParent();
}
