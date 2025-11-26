#include "Global/Rebe_Booth_PlayerState.h"

#include "UE_ReNe.h"
#include "Blueprint/UserWidget.h"

void ARebe_Booth_PlayerState::CreateUI()
{
	if (HasAuthority())
		SHOWWARNF(TEXT("HOST UI GEN"))
	else
		SHOWWARNF(TEXT("CLIENT UI GEN"))
}
