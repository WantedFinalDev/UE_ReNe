// Fill out your copyright notice in the Description page of Project Settings.

#include "Network/Rene_FileUploader.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "GameFramework/PlayerController.h" // Required to get the owning player
#include "GameFramework/PlayerState.h"     // Required to get the player's state and name
#include "Misc/DateTime.h"                 // Required for the timestamp
#include "Global/Rene_GameInstance.h"      // GameInstance 헤더 추가
#include "Misc/CString.h"                  // TCHAR_TO_ANSI 매크로를 위한 헤더 추가

DEFINE_LOG_CATEGORY_STATIC(LogFileUploader, Log, All);

// Sets default values for this component's properties
URene_FileUploader::URene_FileUploader()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // Ticking is not needed for this component
}


// Called when the game starts
void URene_FileUploader::BeginPlay()
{
	Super::BeginPlay();
}

void URene_FileUploader::StartFileUpload(const FString& FilePath, EUploadUserType UserType, const FString& UserId)
{
    UE_LOG(LogFileUploader, Log, TEXT("StartFileUpload called. FilePath: %s, UserType: %d, UserId: %s"), *FilePath, static_cast<int32>(UserType), *UserId);

    // --- GameInstance에서 네트워크 설정 가져오기 ---
    URene_GameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URene_GameInstance>() : nullptr;
    if (!GameInstance)
    {
        UE_LOG(LogFileUploader, Error, TEXT("GameInstance is not valid. Aborting upload."));
        OnFailure.Broadcast(TEXT("GameInstance is not valid."));
        return;
    }
    const FRene_NetworkSettings& NetworkSettings = GameInstance->GetNetworkSettings();
    const FString ServerBaseURL = NetworkSettings.ServerBaseURL;
    // --- 설정 가져오기 끝 ---

    if (ServerBaseURL.IsEmpty())
    {
        UE_LOG(LogFileUploader, Error, TEXT("Server Base URL is not set in DataTable. Aborting upload."));
        OnFailure.Broadcast(TEXT("Server Base URL is not set in DataTable."));
        return;
    }

    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogFileUploader, Error, TEXT("Selected file does not exist at path: %s"), *FilePath);
        OnFailure.Broadcast(TEXT("Selected file does not exist."));
        return;
    }

    // Get Player Name to prepend to the filename
    FString PlayerName = "UnknownPlayer";
    if (APlayerController* OwningController = Cast<APlayerController>(GetOwner()))
    {
        if (APlayerState* PlayerState = OwningController->PlayerState)
        {
            PlayerName = PlayerState->GetPlayerName();
        }
    }
    UE_LOG(LogFileUploader, Log, TEXT("Owner's player name resolved to: %s"), *PlayerName);

    // Get current timestamp
    FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S"));

    // Construct the new filename with player name, timestamp, and original filename
    FString OriginalFilename = FPaths::GetCleanFilename(FilePath);
    FString NewFilename = FString::Printf(TEXT("%s_%s_%s"), *PlayerName, *Timestamp, *OriginalFilename);
    UE_LOG(LogFileUploader, Log, TEXT("New filename for upload: %s"), *NewFilename);

    // 1. Read file data into a byte array (TArray<uint8>)
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        UE_LOG(LogFileUploader, Error, TEXT("Failed to read file data from path: %s"), *FilePath);
        OnFailure.Broadcast(TEXT("Failed to read file data."));
        return;
    }
    UE_LOG(LogFileUploader, Log, TEXT("Successfully read %d bytes from file."), FileData.Num());

    // 2. Determine the correct endpoint and ID field name
    FString FinalURL = ServerBaseURL;
    FString IdFieldName;
    if (UserType == EUploadUserType::Company)
    {
        FinalURL += NetworkSettings.CompanyDocsUploadEndpoint;
        IdFieldName = TEXT("company_id");
    }
    else // JobSeeker
    {
        FinalURL += NetworkSettings.JobSeekerDocsUploadEndpoint;
        IdFieldName = TEXT("jobseeker_id");
    }
    UE_LOG(LogFileUploader, Log, TEXT("Target URL: %s"), *FinalURL);
    UE_LOG(LogFileUploader, Log, TEXT("ID Field Name: %s"), *IdFieldName);

    // 3. Create and configure the HTTP Request
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &URene_FileUploader::OnUploadRequestComplete);

    // 4. Build the multipart/form-data body
    FString Boundary = "---------------------------" + FString::FromInt(FDateTime::Now().GetTicks());
    FString ContentType = "multipart/form-data; boundary=" + Boundary;

    TArray<uint8> RequestContent;
    
    // Add user ID field (company_id or jobseeker_id)
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*("--" + Boundary + "\r\n")), ("--" + Boundary + "\r\n").Len());
    FString UserIdHeader = FString::Printf(TEXT("Content-Disposition: form-data; name=\"%s\"\r\n\r\n"), *IdFieldName);
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*UserIdHeader), UserIdHeader.Len());
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*UserId), UserId.Len());
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(TEXT("\r\n")), FCString::Strlen(TEXT("\r\n")));

    // Add file field
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*("--" + Boundary + "\r\n")), ("--" + Boundary + "\r\n").Len());
    FString FileHeader = "Content-Disposition: form-data; name=\"file\"; filename=\"" + NewFilename + "\"\r\n";
    FileHeader += "Content-Type: application/octet-stream\r\n\r\n";
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*FileHeader), FileHeader.Len());
    RequestContent.Append(FileData);

    // Footer
    FString Footer = "\r\n--" + Boundary + "--\r\n";
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*Footer), Footer.Len());

    // 5. Set Headers and Content
    HttpRequest->SetURL(FinalURL);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), ContentType);
    HttpRequest->SetContent(RequestContent);
    UE_LOG(LogFileUploader, Log, TEXT("HTTP Request created. Content length: %d"), RequestContent.Num());

    // 6. Process the request (asynchronous)
    if (!HttpRequest->ProcessRequest())
    {
        UE_LOG(LogFileUploader, Error, TEXT("Failed to start HTTP request process."));
        OnFailure.Broadcast(TEXT("Failed to start HTTP request."));
    }
    else
    {
        UE_LOG(LogFileUploader, Log, TEXT("HTTP Request processing started."));
    }
}

void URene_FileUploader::OnUploadRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    UE_LOG(LogFileUploader, Log, TEXT("OnUploadRequestComplete fired. bWasSuccessful: %s"), bWasSuccessful ? TEXT("true") : TEXT("false"));

    if (bWasSuccessful && Response.IsValid())
    {
        int32 ResponseCode = Response->GetResponseCode();
        FString ResponseBody = Response->GetContentAsString();
        UE_LOG(LogFileUploader, Log, TEXT("Response received. HTTP Status: %d"), ResponseCode);
        UE_LOG(LogFileUploader, Log, TEXT("Response Body: %s"), *ResponseBody);

        if (EHttpResponseCodes::IsOk(ResponseCode))
        {
            UE_LOG(LogFileUploader, Log, TEXT("Upload successful. Broadcasting OnSuccess."));
            OnSuccess.Broadcast(ResponseBody);
        }
        else
        {
            FString ErrorMsg = FString::Printf(TEXT("HTTP Error: %d, Response: %s"), ResponseCode, *ResponseBody);
            UE_LOG(LogFileUploader, Warning, TEXT("Upload failed with non-OK status. %s"), *ErrorMsg);
            OnFailure.Broadcast(ErrorMsg);
        }
    }
    else
    {
        UE_LOG(LogFileUploader, Error, TEXT("Upload failed. No valid response from server or connection failed."));
        OnFailure.Broadcast(TEXT("No response from server or connection failed."));
    }
}
