// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h" // Required for HTTP
#include "Rene_FileUploader.generated.h"

// Enum to define the type of upload, ensuring the correct endpoint is used.
UENUM(BlueprintType)
enum class EUploadUserType : uint8
{
    Company,
    JobSeeker
};

// Delegate for success, returns the server response body.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFileUploadSuccess, const FString&, ServerResponse);

// Delegate for failure, returns an error message.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFileUploadFailure, const FString&, ErrorMessage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_FileUploader : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URene_FileUploader();

    /**
     * The base URL for the upload server. The final endpoint will be appended to this.
     * Example: "http://127.0.0.1:8000"
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP")
    FString ServerBaseURL = "http://100.112.161.59:8000";

    /** Delegate broadcasted on successful upload. */
    UPROPERTY(BlueprintAssignable, Category = "HTTP")
    FOnFileUploadSuccess OnSuccess;

    /** Delegate broadcasted on failed upload. */
    UPROPERTY(BlueprintAssignable, Category = "HTTP")
    FOnFileUploadFailure OnFailure;

    /**
     * Initiates the file upload process. This is the main entry point for the component.
     * Reads the file asynchronously and sends it via HTTP POST.
     * @param FilePath The absolute path to the file on the user's machine.
     * @param UserType The type of user uploading, to determine the API endpoint.
     */
    UFUNCTION(BlueprintCallable, Category = "File Uploader")
    void StartFileUpload(const FString& FilePath, EUploadUserType UserType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    /**
     * The internal callback for when the HTTP request completes.
     * @param Request The original request object.
     * @param Response The response from the server.
     * @param bWasSuccessful True if the request was successfully processed at the transport layer.
     */
    void OnUploadRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    // Endpoint paths
    const FString CompanyEndpoint = TEXT("/api/v1/upload/company-docs");
    const FString JobSeekerEndpoint = TEXT("/api/v1/upload/jobseeker-docs");
};
