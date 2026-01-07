#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_UploadingPopupWidget.generated.h"

class UWidgetSwitcher;

UCLASS()
class UE_RENE_API URene_UploadingPopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	void ShowUploadingState();
	void ShowCompleteState();
	void ShowErrorState();
	void ShowWidget();
	void HideWidget();
		


private:
	void SetHideTimer();




	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;
	
	// UPROPERTY(meta=(BindWidget))
	// TObjectPtr<>




private:
	FTimerHandle HideTimerHandle;




};
