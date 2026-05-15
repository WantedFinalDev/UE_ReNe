
#include "FE/Widget_ActivatableBase.h"

#include "UE_ReNe.h"
#include "FE/FE_PlayerController.h"
#include "Player/Rene_PlayerController.h"

AFE_PlayerController* UWidget_ActivatableBase::GetOwningFrontEndPC()
{
	if (!CachedOwnPC.IsValid())
		CachedOwnPC = GetOwningPlayer<AFE_PlayerController>();
		
	if (CachedOwnPC.IsValid())
		return CachedOwnPC.Get();
	else
		return nullptr;
}

ARene_PlayerController* UWidget_ActivatableBase::GetOwningRenePC()
{
	if (!CachedRenePC.IsValid())
		CachedRenePC = GetOwningPlayer<ARene_PlayerController>();
		
	if (CachedRenePC.IsValid())
		return CachedRenePC.Get();
	else
	{
		LOGERRORF(TEXT("Rene Controller has Fucked"));
		return nullptr;
	}
}
