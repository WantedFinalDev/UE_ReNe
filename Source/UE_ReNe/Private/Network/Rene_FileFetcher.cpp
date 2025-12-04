// Copyright Epic Games, Inc. All Rights Reserved.

#include "Public/Network/Rene_FileFetcher.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2D.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

URene_FileFetcher::URene_FileFetcher()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URene_FileFetcher::StartDownloadPng()
{
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();

	HttpRequest->SetURL(PngUrl);
	HttpRequest->SetVerb(TEXT("GET"));
	
	// Bind the callback function
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URene_FileFetcher::OnHttpRequestComplete);

	// Process the request
	if (!HttpRequest->ProcessRequest())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to start HTTP request to %s"), *PngUrl);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Started HTTP request to %s"), *PngUrl);
	}
}

void URene_FileFetcher::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed. Response was not valid."));
		return;
	}

	if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP request returned error code: %d"), Response->GetResponseCode());
		return;
	}

	// Get image data
	const TArray<uint8>& PngData = Response->GetContent();

	if (PngData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP response was empty."));
		return;
	}

	// Load the image wrapper module
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	
	// Detect image format
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(PngData.GetData(), PngData.Num());
	if (ImageFormat != EImageFormat::PNG)
	{
		UE_LOG(LogTemp, Error, TEXT("Downloaded file is not a PNG. Detected format: %d"), (int32)ImageFormat);
		return;
	}

	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(PngData.GetData(), PngData.Num()))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create image wrapper or set compressed data."));
		return;
	}

	// Get raw pixel data
	TArray<uint8> RawData;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to decompress PNG to raw BGRA data."));
		return;
	}

	if (RawData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Decompressed PNG data is empty."));
		return;
	}

	// Create a transient Texture2D
	const int32 Width = ImageWrapper->GetWidth();
	const int32 Height = ImageWrapper->GetHeight();
	
	UTexture2D* NewTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
	if (!NewTexture)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create transient UTexture2D."));
		return;
	}

#if WITH_EDITORONLY_DATA
	NewTexture->MipGenSettings = TMGS_NoMipmaps;
#endif
	NewTexture->NeverStream = true;
	NewTexture->SRGB = true;

	// Copy pixel data to the texture
	void* TextureData = NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	if (TextureData)
	{
		FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
		NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		
		// Update the texture resource
		NewTexture->UpdateResource();

		UE_LOG(LogTemp, Log, TEXT("Successfully created Texture2D from downloaded PNG."));
		
		// Broadcast the delegate with the new texture
		OnPngDownloadSuccess.Broadcast(NewTexture);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to lock texture memory for writing."));
	}
}