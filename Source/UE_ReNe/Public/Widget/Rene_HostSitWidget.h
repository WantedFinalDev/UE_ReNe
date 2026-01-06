// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_HostSitWidget.generated.h"

class UButton;

UCLASS()
class UE_RENE_API URene_HostSitWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_SitHost;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_Back;

	UFUNCTION()
	void OnSitHostClicked();

	UFUNCTION()
	void OnBackClicked();
};
