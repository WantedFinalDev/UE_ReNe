#pragma once

#include "CoreMinimal.h"
#include "FE/Widget_ActivatableBase.h"
#include "Widget_Join_conv.generated.h"

class UScrollBox;
class UButton;
class URene_SessionInfoWidget;

UCLASS()
class UE_RENE_API UWidget_Join_conv : public UWidget_ActivatableBase
{
	GENERATED_BODY()

	/* Method */
public:
	UFUNCTION()
	void OnFindComplete(int idx, FString str);
	
	UFUNCTION()
	void OnClickedFind();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Find;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> scr_SessionList;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URene_SessionInfoWidget> session_info_widget;

private:
};
