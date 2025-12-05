// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_FileFetcher.generated.h"

// Delegate to be called when the PNG download and texture creation is successful.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPngDownloadSuccess, UTexture2D*, DownloadedTexture);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_FileFetcher : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URene_FileFetcher();

	/**
	 * Delegate called on successful PNG download and conversion to Texture2D.
	 */
	UPROPERTY(BlueprintAssignable, Category = "FileFetcher")
	FOnPngDownloadSuccess OnPngDownloadSuccess;

	/**
	 * Starts the process of downloading the PNG from the fixed URL.
	 * Can be called from a Blueprint, e.g., on a UI button click.
	 */
	UFUNCTION(BlueprintCallable, Category = "FileFetcher")
	void StartDownloadPng();

protected:
	/**
	 * Callback function for when the HTTP request is completed.
	 * @param Request The original HTTP request.
	 * @param Response The HTTP response from the server.
	 * @param bWasSuccessful Whether the request was successfully processed.
	 */
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	/** The hardcoded URL for the PNG file. */
	const FString PngUrl = TEXT("http://100.112.161.59:8000/api/v1/p2p/audio-chunk2");
};