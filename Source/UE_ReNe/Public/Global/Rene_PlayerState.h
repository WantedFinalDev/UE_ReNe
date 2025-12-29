#pragma once

#include "CoreMinimal.h"
#include "Data/ReneUserData.h"
#include "GameFramework/PlayerState.h"
#include "Rene_PlayerState.generated.h"

/*
 *
 */
UCLASS()
class UE_RENE_API ARene_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
	/* Method */
public:
	ARene_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FReneUserData GetReneUserData() const { return userdata; }
	FString GetReneUserName() const { return userdata.Name; }
	FString GetReneUserId() const { return userdata.ID; }
	int32 GetReneUserLevel() const { return userdata.Level; }
	
	void SetReneUserData(const FReneUserData& data) { userdata = data; }
	void SetReneUserName(const FString& name) { userdata.Name = name; }
	
	// New getter for voice chat eligibility
	bool IsInPrivateInterview() const { return bVoicable; }
	
	// Setter for voice chat eligibility
	void SetIsInPrivateInterview(bool bNewState);

	// Getter and Setter for the AI Interview Result ID
	void SetInterviewResultID(int32 ResultID);
	int32 GetInterviewResultID() const;
	
	
private:
	
	
	
	
	
	
	/* Field */
public:
	// Flag to indicate if this player is currently in a private interview and allowed to use P2P voice chat
	UPROPERTY(Replicated)
	bool bVoicable;
	
	
	
	
private:
	UPROPERTY(Replicated)
	FReneUserData userdata;

	// The result ID of the completed AI interview. Not replicated to other clients.
	int32 InterviewResultID;
	
	
	
	
};
