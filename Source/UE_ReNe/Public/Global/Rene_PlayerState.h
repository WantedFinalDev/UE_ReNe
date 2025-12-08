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
	ARene_Booth_PlayerState(); // Constructor

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FReneUserData GetReneUserData() const { return userdata; }
	FString GetReneUserName() const { return userdata.Name; }
	FString GetReneUserId() const { return userdata.ID; }
	int32 GetReneUserLevel() const { return userdata.Level; }
	
	void SetReneUserData(const FReneUserData& data) { userdata = data; }
	void SetReneUserName(const FString& name) { userdata.Name = name; }
	

	// New getter for voice chat eligibility
	bool IsInPrivateInterview() const { return bIsInPrivateInterview; }
	
	
private:
	
	
	
	
	
	
	/* Field */
public:
	// Flag to indicate if this player is currently in a private interview and allowed to use P2P voice chat
	UPROPERTY(Replicated)
	bool bIsInPrivateInterview;
	
	
	
	
private:
	UPROPERTY(Replicated)
	FReneUserData userdata;
	
	
	
	
	
};
