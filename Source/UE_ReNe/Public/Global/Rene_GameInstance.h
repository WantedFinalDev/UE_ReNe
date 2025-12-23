#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Data/ReneUserData.h"
#include "Data/Rene_NetworkSettings.h" // 데이터 테이블 구조체 헤더 포함
#include "Network/Rene_LoginManager.h" // LoginManager 헤더 포함
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
	UFUNCTION(BlueprintCallable)
	void DestroyReneSession();
	
	// Login Process
	void SetReneUserData(const FString& id, const FString& pw, const FString& name, const int32 level, const FString& role);
	FReneUserData GetCachedUserData() { return f_userdata; }

	// 데이터 테이블에서 네트워크 설정을 가져오는 Getter 함수
	UFUNCTION(BlueprintPure, Category = "Rene|Network")
	const FRene_NetworkSettings& GetNetworkSettings() const;

	// Login Request Wrapper
	UFUNCTION(BlueprintCallable, Category = "Rene|Network")
	void RequestLogin(const FString& Email, const FString& Password, const FString& Role);

	// Delegate to forward login results to UI
	UPROPERTY(BlueprintAssignable, Category = "Rene|Network")
	FReneOnLoginCompleteDelegate OnLoginComplete;
	
private:
	void OnCreateReneSession(FName sessionname, bool b_success);
	void OnFindReneSession(bool b_success);
	void OnJoinReneSession(FName session_name, EOnJoinSessionCompleteResult::Type result);

	UFUNCTION()
	void OnLoginManagerComplete(bool bSuccess, const FReneUserData& UserData, FString ErrorMessage);
	
	
	/* Field */
public:
	IOnlineSessionPtr p_ReneSessionInterface;
	
	TSharedPtr<class FOnlineSessionSearch> p_ReneSessionSearch;
	
	FOnFindReneSessionCompleteDelegate OnFindReneSessionComplete;
	
protected:
	// 블루프린트에서 설정할 데이터 테이블 애셋을 가리키는 포인터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rene|Data")
	TObjectPtr<UDataTable> NetworkSettingsTable;
	
private:
	UPROPERTY()
	FReneUserData f_userdata;

	// 로드된 네트워크 설정을 저장할 변수
	UPROPERTY()
	FRene_NetworkSettings CachedNetworkSettings;

	UPROPERTY()
	TObjectPtr<URene_LoginManager> LoginManager;
};
