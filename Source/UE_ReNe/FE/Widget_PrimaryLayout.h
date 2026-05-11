#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class UE_RENE_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InStackTag) const;

		
protected:
	// UPARAM~ : 
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "FE.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStackWidget);	


private:





	/* Field */
public:





private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;




};
