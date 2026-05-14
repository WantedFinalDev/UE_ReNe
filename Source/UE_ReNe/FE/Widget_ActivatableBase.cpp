
#include "FE/Widget_ActivatableBase.h"
#include "FE/FE_PlayerController.h"

AFE_PlayerController* UWidget_ActivatableBase::GetOwningFrontEndPC()
{
	if (!CachedOwnPC.IsValid())
		CachedOwnPC = GetOwningPlayer<AFE_PlayerController>();
		
	if (CachedOwnPC.IsValid())
		return CachedOwnPC.Get();
	else
		return nullptr;
}
