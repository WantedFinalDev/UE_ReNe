#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_StartWidget.generated.h"


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
	TObjectPtr<UButton> btn_Enter;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Exit;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_ID;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_PW;
	
	
	
private:
	
	
	
};
