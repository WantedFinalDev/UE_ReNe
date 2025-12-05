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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FReneUserData GetReneUserData() const { return userdata; }
	FString GetReneUserName() const { return userdata.Name; }
	FString GetReneUserId() const { return userdata.ID; }
	int32 GetReneUserLevel() const { return userdata.Level; }
	
	void SetReneUserData(const FReneUserData& data) { userdata = data; }
	void SetReneUserName(const FString& name) { userdata.Name = name; }
	
	
	
private:
	
	
	
	
	
	
	/* Field */
public:
	
	
	
	
	
private:
	UPROPERTY(Replicated)
	FReneUserData userdata;
	
	
	
	
	
};
