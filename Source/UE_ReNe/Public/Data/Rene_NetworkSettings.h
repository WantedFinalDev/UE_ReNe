#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Rene_NetworkSettings.generated.h"

/**
 * Defines the structure for a row in the Network Settings Data Table.
 * All network-related URLs and endpoints should be defined here.
 */
USTRUCT(BlueprintType)
struct FRene_NetworkSettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** The base URL for the server (e.g., "http://100.112.161.59:8000") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString ServerBaseURL;

	/** The endpoint for company document uploads (e.g., "/api/v1/upload/company-docs") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString CompanyDocsUploadEndpoint;

	/** The endpoint for job seeker document uploads (e.g., "/api/v1/upload/jobseeker-docs") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString JobSeekerDocsUploadEndpoint;

	/** The full URL for downloading the report image (e.g., "http://.../audio-chunk2") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString ReportImageDownloadURL;

	/** The full URL for uploading recorded voice data (e.g., "http://.../audio-check") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString VoiceDataUploadURL;

	/** The full URL for starting an AI interview (e.g., "/api/v1/company/ai-interview/start") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString AIInterviewStartURL;

	/** The full URL for AI interview chat voice responses (e.g., "/api/v1/company/ai-interview/chat/voice") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network Settings")
	FString AIInterviewChatURL;
};
