#pragma once

#include "CoreMinimal.h"
#include "FE/Widget_ActivatableBase.h"
#include "Data/ReneUserData.h"
#include "Widget_Login_conv.generated.h"

class UWidgetSwitcher;
class UCheckBox;
class UEditableTextBox;
class UTextBlock;
class UButton;

UCLASS()
class UE_RENE_API UWidget_Login_conv : public UWidget_ActivatableBase
{
	GENERATED_BODY()

	/* Method */
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoginResponse(bool bSuccess, const FReneUserData& UserData, const FString& ErrorMessage);
	
	virtual void OnLoginResponse_Implementation(bool bSuccess, const FReneUserData& UserData, const FString& ErrorMessage);
	
protected:
	virtual void NativeOnInitialized() override;
	
	
	
private:
	UFUNCTION()
	void OnClickedEnter();
	UFUNCTION()
	void OnClickedExit();
	

	
	
	
	
	/* Field */
public:	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Enter;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Exit;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_ID;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_PW;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> cbox_Company;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> cbox_Seeker;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_Error;
	
	
private:
};
