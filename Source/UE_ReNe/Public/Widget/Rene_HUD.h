#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_HUD.generated.h"

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

	
	
private:
	/* Field */
	
	
	
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Home;

	
	
private:
	
	
	
	
	
};
