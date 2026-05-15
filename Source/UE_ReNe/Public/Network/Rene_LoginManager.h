#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Data/ReneUserData.h"
#include "Rene_LoginManager.generated.h"

// Renamed delegate to avoid conflict with OnlineIdentityInterface.h
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReneOnLoginCompleteDelegate, bool, bSuccess, const FReneUserData&, UserData, const FString&, ErrorMessage);

/**
 * Manages user login via HTTP requests.
 */
UCLASS()
class UE_RENE_API URene_LoginManager : public UObject
{
	GENERATED_BODY()

public:
	URene_LoginManager();

	// Initiates a login request.
	void RequestLogin(const FString& Email, const FString& Password, const FString& Role, const FString& BaseURL, const FString& Endpoint);

	// Delegate to broadcast login results.
	UPROPERTY(BlueprintAssignable, Category = "Network|Login")
	FReneOnLoginCompleteDelegate OnLoginComplete;

private:
	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString CurrentRole; // To distinguish between JobSeeker and Company response parsing
};
