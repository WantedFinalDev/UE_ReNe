
#include "FE/FE_FunctionLibrary.h"

#include "OnlineSubsystemTypes.h"
#include "FE/FE_DeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFE_FunctionLibrary::GetFESoftWidgetClassByTag(UPARAM(meta = (Categories = "FE.Widget"))FGameplayTag InWidgetTag)
{
	const UFE_DeveloperSettings* DevSetting = GetDefault<UFE_DeveloperSettings>();
	
	checkf(DevSetting->FE_Widget_Map.Contains(InWidgetTag), TEXT("Cannot Found Tag %s"), *InWidgetTag.ToString());
	
	return DevSetting->FE_Widget_Map.FindRef(InWidgetTag);
}
