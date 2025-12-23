#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Rene_NetworkSettings.generated.h"

USTRUCT(BlueprintType)
struct FRene_NetworkSettings : public FTableRowBase
{
    GENERATED_BODY()

    // The base URL for the server.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString ServerBaseURL;

    // The URL to upload recorded voice data for transcription and storage.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString VoiceDataUploadURL;

    // The URL to start a new AI interview session.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString AIInterviewStartURL;

    // The URL to send voice data during an AI interview.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString AIInterviewChatURL;

    // The base URL for viewing AI interview reports.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString AIReportURL;

    // Endpoint for company document uploads.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString CompanyDocsUploadEndpoint;

    // Endpoint for job seeker document uploads.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString JobSeekerDocsUploadEndpoint;

    // URL for downloading report images.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString ReportImageDownloadURL;

    // Endpoint for job seeker login.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString JobSeekerLoginEndpoint;

    // Endpoint for company login.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString CompanyLoginEndpoint;
};
