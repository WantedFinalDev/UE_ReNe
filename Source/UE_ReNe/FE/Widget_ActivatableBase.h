#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class ARene_PlayerController;
class AFE_PlayerController;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class UE_RENE_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	UFUNCTION(BlueprintPure)
	AFE_PlayerController* GetOwningFrontEndPC();
	
	UFUNCTION(BlueprintPure)
	ARene_PlayerController* GetOwningRenePC();
		


private:





	/* Field */
public:





private:
	TWeakObjectPtr<AFE_PlayerController> CachedOwnPC;
	TWeakObjectPtr<ARene_PlayerController> CachedRenePC;
	



};
