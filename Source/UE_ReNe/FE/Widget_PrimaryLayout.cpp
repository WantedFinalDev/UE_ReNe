
#include "FE/Widget_PrimaryLayout.h"
#include "FE/FE_DubugHelper.h"


UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InStackTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InStackTag),TEXT("Can Not Found Tag %s"), *InStackTag.ToString());
	
	return RegisteredWidgetStackMap.FindRef(InStackTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "FE.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStackWidget)
{
	if (!IsDesignTime())	// Runtime Check
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStackWidget);
		}
	}
}
