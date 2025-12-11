// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_FileFetcher.generated.h"

// Delegate to be called when the PNG download and texture creation is successful.
DECLARE_DELEGATE_OneParam(FOnFileFetchSuccess, UTexture2D*);

UCLASS()
class UE_RENE_API URene_FileFetcher : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URene_FileFetcher();

	/**
	 * Delegate called on successful PNG download and conversion to Texture2D.
	 */
	FOnFileFetchSuccess OnFileFetchSuccess;

	/**
	 * Starts the process of downloading the PNG from the given URL.
	 */
	void FetchFile(const FString& Url);

protected:
	/**
	 * Callback function for when the HTTP request is completed.
	 * @param Request The original HTTP request.
	 * @param Response The HTTP response from the server.
	 * @param bWasSuccessful Whether the request was successfully processed.
	 */
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
