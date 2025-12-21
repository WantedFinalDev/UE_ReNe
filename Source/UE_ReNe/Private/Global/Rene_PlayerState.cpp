#include "Global/Rene_PlayerState.h"

#include "UE_ReNe.h"
#include "Net/UnrealNetwork.h"

ARene_PlayerState::ARene_PlayerState()
{
	// Default to not being in an interview
	bVoicable = false;
	InterviewResultID = 0; // Initialize with a non-valid ID
}

void ARene_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARene_PlayerState, userdata);
	DOREPLIFETIME(ARene_PlayerState, bVoicable); // Replicate the new flag
}

void ARene_PlayerState::SetInterviewResultID(int32 ResultID)
{
	InterviewResultID = ResultID;
}

int32 ARene_PlayerState::GetInterviewResultID() const
{
	return InterviewResultID;
}
