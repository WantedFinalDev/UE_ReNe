#include "Global/Rene_Booth_PlayerState.h"

#include "UE_ReNe.h"
#include "Net/UnrealNetwork.h"

ARene_Booth_PlayerState::ARene_Booth_PlayerState()
{
	// Default to not being in an interview
	bIsInPrivateInterview = false;
}

void ARene_Booth_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARene_Booth_PlayerState, userdata);
	DOREPLIFETIME(ARene_Booth_PlayerState, bIsInPrivateInterview); // Replicate the new flag
}
