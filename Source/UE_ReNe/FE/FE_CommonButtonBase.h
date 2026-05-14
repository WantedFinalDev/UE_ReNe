#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FE_CommonButtonBase.generated.h"

class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class UE_RENE_API UFE_CommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

	/* Method */
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);

protected:
	virtual void NativePreConstruct() override;
	
	/* Field */
public:


private:
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Front End Button", meta=(AllowPrivateAccess))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Front End Button", meta=(AllowPrivateAccess))
	bool bUseUppercaseButtonText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Front End Button", meta=(AllowPrivateAccess))
	FText ButtonDescriptionText;
	
	
	
};
