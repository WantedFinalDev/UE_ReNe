#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FE_DeveloperSettings.generated.h"

class UWidget_ActivatableBase;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "FrontEnd UI Settings"))
class UE_RENE_API UFE_DeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	/* Method */
public:

		


private:





	/* Field */
public:
	UPROPERTY(Config, EditAnywhere, Category="Widget Reference", meta = (ForceInlineRow, Categories = "FE.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FE_Widget_Map;




private:





};
