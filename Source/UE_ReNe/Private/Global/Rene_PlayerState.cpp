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
	DOREPLIFETIME(ARene_PlayerState, InterviewResultID);
}

void ARene_PlayerState::SetIsInPrivateInterview(bool bNewState)
{
	if (HasAuthority())
	{
		bVoicable = bNewState;
	}
}

void ARene_PlayerState::SetInterviewResultID(int32 ResultID)
{
	if (HasAuthority())
	{
		InterviewResultID = ResultID;
		
		// 서버(Host)에서는 OnRep이 자동 호출되지 않으므로 수동으로 호출하여
		// Host 플레이어도 델리게이트 이벤트를 받을 수 있게 함
		OnRep_InterviewResultID();
	}
}

int32 ARene_PlayerState::GetInterviewResultID() const
{
	return InterviewResultID;
}

void ARene_PlayerState::OnRep_InterviewResultID()
{
	OnInterviewResultIDUpdated.Broadcast(InterviewResultID);
}
