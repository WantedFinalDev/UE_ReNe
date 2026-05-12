#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FE_FunctionLibrary.generated.h"

class UWidget_ActivatableBase;

UCLASS()
class UE_RENE_API UFE_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/* Method */
public:
	UFUNCTION(BlueprintPure, Category = "FE Function Library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFESoftWidgetClassByTag(UPARAM(meta = (Categories = "FE.Widget")) FGameplayTag InWidgetTag);


private:





	/* Field */
public:





private:





};
