#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_Seeker_Widget.generated.h"


class UButton;

UCLASS()
class UE_RENE_API URene_Seeker_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	
	
private:
	UFUNCTION()
	void OnClickedClose();	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Close;	
	
	
	
private:
	
	
	
	
	
};
