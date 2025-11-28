#include "Global/Rene_Booth_PlayerState.h"

#include "UE_ReNe.h"
#include "Net/UnrealNetwork.h"


void ARene_Booth_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARene_Booth_PlayerState, userdata);
}
