#include "Network/Rene_LoginManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "UE_ReNe.h" // 로그 매크로 사용을 위해 추가

URene_LoginManager::URene_LoginManager()
{
}

void URene_LoginManager::RequestLogin(const FString& Email, const FString& Password, const FString& Role, const FString& BaseURL, const FString& Endpoint)
{
	CurrentRole = Role;

	FHttpModule* Http = &FHttpModule::Get();
	if (!Http)
	{
		OnLoginComplete.Broadcast(false, FReneUserData(), TEXT("HTTP Module not available."));
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	FString FullURL = BaseURL + Endpoint;
	Request->SetURL(FullURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Create JSON Body
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("email"), Email);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString Content;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindUObject(this, &URene_LoginManager::OnLoginResponseReceived);

	if (!Request->ProcessRequest())
	{
		OnLoginComplete.Broadcast(false, FReneUserData(), TEXT("Failed to process HTTP request."));
	}
}

void URene_LoginManager::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnLoginComplete.Broadcast(false, FReneUserData(), TEXT("Connection failed."));
		return;
	}

	if (Response->GetResponseCode() != 200)
	{
		OnLoginComplete.Broadcast(false, FReneUserData(), FString::Printf(TEXT("Server error: %d"), Response->GetResponseCode()));
		return;
	}

	FString ResponseBody = Response->GetContentAsString();
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		FReneUserData ParsedUserData;
		
		// Common fields
		ParsedUserData.Name = JsonObject->GetStringField(TEXT("name"));
		// Password is not returned by server, but we might want to keep it locally or leave it empty.
		// For now, we leave it as "EmptyPW" or whatever default, as it's not in the response.
		// If needed, we could pass the password from the request, but security-wise it's better not to store it if not needed.
		
		ParsedUserData.Role = JsonObject->GetStringField(TEXT("user_type"));

		if (CurrentRole == TEXT("jobseeker"))
		{
			int32 UserId = JsonObject->GetIntegerField(TEXT("user_id"));
			ParsedUserData.ID = FString::FromInt(UserId);
			ParsedUserData.Level = 1; // Default level
		}
		else if (CurrentRole == TEXT("company"))
		{
			int32 CompanyId = JsonObject->GetIntegerField(TEXT("company_id"));
			ParsedUserData.ID = FString::FromInt(CompanyId);
			
			// Use company_id as Level if not specified otherwise
			ParsedUserData.Level = CompanyId; 
			
			// [수정] 오타(jop_group_id) 제거 및 정석 필드명(job_group_id) 사용
			if (JsonObject->HasField(TEXT("job_group_id")))
			{
				ParsedUserData.JobGroupID = JsonObject->GetIntegerField(TEXT("job_group_id"));
			}
			else
			{
				// Fallback if not exists
				ParsedUserData.JobGroupID = 0; 
			}

			// [추가] Company 로그인 시 JobGroupID 파싱 결과 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("LoginManager: Company Login Parsed - ID: %s, JobGroupID: %d"), *ParsedUserData.ID, ParsedUserData.JobGroupID);
		}
		else
		{
			// Fallback or error
			ParsedUserData.ID = TEXT("UnknownID");
		}

		OnLoginComplete.Broadcast(true, ParsedUserData, TEXT(""));
	}
	else
	{
		OnLoginComplete.Broadcast(false, FReneUserData(), TEXT("Failed to parse JSON response."));
	}
}
