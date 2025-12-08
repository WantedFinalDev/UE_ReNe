#include "Global/Rene_PlayerState.h"

#include "UE_ReNe.h"
#include "Net/UnrealNetwork.h"


void ARene_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARene_PlayerState, userdata);
}

