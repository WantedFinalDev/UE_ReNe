#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_Booth_Widget.generated.h"


UCLASS()
class UE_RENE_API URene_Booth_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	
	
private:
	UFUNCTION()
	void OnClickedNoti();
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> btn_Noti;
	
	
	
	
private:
	
	
	
	
	
};
