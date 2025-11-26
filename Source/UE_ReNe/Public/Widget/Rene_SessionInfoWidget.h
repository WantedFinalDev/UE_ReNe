#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Rene_SessionInfoWidget.generated.h"


class UButton;

UCLASS()
class UE_RENE_API URene_SessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	void SetSessionInfo(int32 idx, FString session_name);
	
	
	
	
private:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickedJoin();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Join;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_SessionName;
	
	
	
	
	
private:
	int32 info_idx = 0;
	
	
	
};
