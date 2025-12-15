#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Rene_StartWidget.generated.h"


class UWidgetSwitcher;
class URene_LobbyWidget;
class UEditableTextBox;
class UButton;

UCLASS()
class UE_RENE_API URene_StartWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	
	
private:
	UFUNCTION()
	void OnClickedEnter();
	UFUNCTION()
	void OnClickedExit();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> sw_Switcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Enter;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Exit;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_ID;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_LobbyWidget> WBP_LobbyUI;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> cbox_Company;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> cbox_Seeker;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_Error;
	
private:
	
	
	
};
