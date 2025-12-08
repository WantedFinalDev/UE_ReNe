#include "Global/Rene_PlayerState.h"

#include "UE_ReNe.h"
#include "Net/UnrealNetwork.h"

ARene_PlayerState::ARene_PlayerState()
{
	// Default to not being in an interview
	bIsInPrivateInterview = false;
}

void ARene_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARene_PlayerState, userdata);
	DOREPLIFETIME(ARene_PlayerState, bIsInPrivateInterview); // Replicate the new flag
}
