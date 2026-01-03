#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_HUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedHome);

class ARene_PlayerController;
class UWidgetSwitcher;
class UButton;

UCLASS()
class UE_RENE_API URene_HUD : public UUserWidget
{
	GENERATED_BODY()

	/* Method */
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickedHome();
	UFUNCTION()
	void OnClickedProfile();
	UFUNCTION()
	void OnClickedMeeting();
	UFUNCTION()
	void OnClickedClose();

	
	
private:
	/* Field */
	
	
	
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> sw_HUD;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Home;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>	btn_HQ_Profile;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_HQ_Meeting;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_HQ_Close;
	
	UPROPERTY(BlueprintAssignable)
	FOnClickedHome OnClickedHomeDelegate;
	
	
	
	
private:
	UPROPERTY()
	TObjectPtr<ARene_PlayerController> p_OwnerController;
	
	
	
	
};
