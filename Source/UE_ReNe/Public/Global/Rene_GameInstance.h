#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Data/ReneUserData.h"
#include "Rene_GameInstance.generated.h"


using FOnFindReneSessionCompleteDelegate = TMulticastDelegate<void (int32, FString)>;

UCLASS()
class UE_RENE_API URene_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void CreateReneSession(int32 n_maxplayer, FString s_sessionname);
	UFUNCTION(BlueprintCallable)
	void JoinReneSession(int32 idx);
	UFUNCTION(BlueprintCallable)
	void FindReneSession();
	
	// Login Process
	void SetReneUserData(const FString& id, const FString& name, const int32 level);
	FReneUserData GetCachedUserData() { return f_userdata; }
	
private:
	void OnCreateReneSession(FName sessionname, bool b_success);
	void OnFindReneSession(bool b_success);
	void OnJoinReneSession(FName session_name, EOnJoinSessionCompleteResult::Type result);
	
	
	/* Field */
public:
	IOnlineSessionPtr p_ReneSessionInterface;
	
	TSharedPtr<class FOnlineSessionSearch> p_ReneSessionSearch;
	
	FOnFindReneSessionCompleteDelegate OnFindReneSessionComplete;
	
	
	
private:
	UPROPERTY()
	FReneUserData f_userdata;
	
	
	
};
