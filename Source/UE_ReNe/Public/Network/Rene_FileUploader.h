#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_FileUploader.generated.h"

// Enum to specify the user type for the upload
UENUM(BlueprintType)
enum class EUploadUserType : uint8
{
    Company,
    JobSeeker
};

// Delegate for successful upload
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFileUploadSuccess, const FString&, ResponseBody);

// Delegate for failed upload
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFileUploadFailure, const FString&, ErrorMessage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RENE_API URene_FileUploader : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URene_FileUploader();

	// Function to start the file upload process
	UFUNCTION(BlueprintCallable, Category = "File Uploader")
	void StartFileUpload(const FString& FilePath, EUploadUserType UserType, const FString& UserId);

	// Delegates for upload events
	UPROPERTY(BlueprintAssignable, Category = "File Uploader")
	FOnFileUploadSuccess OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "File Uploader")
	FOnFileUploadFailure OnFailure;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Callback for when the HTTP request completes
	void OnUploadRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
