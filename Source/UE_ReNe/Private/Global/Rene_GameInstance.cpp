#include "Global/Rene_GameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "UE_ReNe.h"
#include "Online/OnlineSessionNames.h"

void URene_GameInstance::Init()
{
	Super::Init();
	
	IOnlineSubsystem* sys = Online::GetSubsystem(GetWorld());
	if (sys == nullptr) return;
	p_ReneSessionInterface = sys->GetSessionInterface();
	
	if (p_ReneSessionInterface.IsValid())
	{
		p_ReneSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URene_GameInstance::OnCreateReneSession);
		p_ReneSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URene_GameInstance::OnFindReneSession);
		p_ReneSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URene_GameInstance::OnJoinReneSession);
	}
}

void URene_GameInstance::CreateReneSession(int32 n_maxplayer, FString s_sessionname)
{
	FOnlineSessionSettings settings;
	FName sysname = Online::GetSubsystem(GetWorld())->GetSubsystemName();
	LOGWARNF(TEXT("Subsystem : %s"), *sysname.ToString());
	
	settings.bIsLANMatch = sysname.IsEqual(TEXT("NULL"));
	settings.NumPublicConnections = n_maxplayer;
	settings.bShouldAdvertise = true;
	settings.bAllowJoinInProgress = true;
	settings.bUseLobbiesIfAvailable = true;
	settings.bUsesPresence = true;
	settings.Set(FName(TEXT("GAMEID")), FString(TEXT("UE_ReNe")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	settings.Set(FName(TEXT("ROOMNAME")), s_sessionname, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	
	if (p_ReneSessionInterface == nullptr) return;
	FUniqueNetIdPtr netid = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	p_ReneSessionInterface->CreateSession(*netid, NAME_GameSession, settings);
}

void URene_GameInstance::JoinReneSession(int32 idx)
{
	if (!p_ReneSessionSearch.IsValid() || !p_ReneSessionInterface.IsValid())
	{
		LOGERRORF(TEXT("Join FuckedUp"));
		return;
	}
	FOnlineSessionSearchResult selected = p_ReneSessionSearch->SearchResults[idx];
	selected.Session.SessionSettings.bUseLobbiesIfAvailable = true;
	selected.Session.SessionSettings.bUsesPresence = true;
	
	FString str; 
	selected.Session.SessionSettings.Get(FName(TEXT("ROOMNAME")), str);
	LOGWARNF(TEXT("Joining %s Room"), *str)
	p_ReneSessionInterface->JoinSession(0, NAME_GameSession, selected);
}

void URene_GameInstance::FindReneSession()
{	
	UE_LOG(LogTemp, Warning, TEXT("Start Find Session"));
	p_ReneSessionSearch = MakeShared<FOnlineSessionSearch>();
	
	FName sysname = Online::GetSubsystem(GetWorld())->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("Subsystem : %s"), *sysname.ToString());
	
	p_ReneSessionSearch->bIsLanQuery = sysname.IsEqual(TEXT("NULL"));
	p_ReneSessionSearch->MaxSearchResults = 100;
	if (!sysname.IsEqual(TEXT("NULL")))
	{
		p_ReneSessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		p_ReneSessionSearch->QuerySettings.Set(FName(TEXT("GAMEID")), FString(TEXT("UE_ReNe")), EOnlineComparisonOp::Equals);
	}
	
	if (!p_ReneSessionInterface.IsValid()) return;
	p_ReneSessionInterface->FindSessions(0, p_ReneSessionSearch.ToSharedRef());
}

void URene_GameInstance::SetReneUserData(const FString& id, const FString& name, const int32 level)
{
	f_userdata.ID = id;
	f_userdata.Name = name;
	f_userdata.Level = level;
	
	LOGWARNF(TEXT("UserData Cached : %s"), *f_userdata.Name);
}

void URene_GameInstance::OnCreateReneSession(FName sessionname, bool b_success)
{
	FName sysname = Online::GetSubsystem(GetWorld())->GetSubsystemName();
	LOGWARNF(TEXT("Online Sub System : %s"), *sysname.ToString());

	if (b_success)
	{
		LOGWARNF(TEXT("%s Session Created"), *sessionname.ToString());
		 GetWorld()->ServerTravel(TEXT("/Game/Maps/BoothMap?listen"));
	}
	else
	{
		LOGWARNF(TEXT("%s Session Creation Failed"), *sessionname.ToString());
	}
}

void URene_GameInstance::OnFindReneSession(bool b_success)
{
	LOGWARNF(TEXT("End Find Session"));

	if (!b_success)
	{
		LOGERRORF(TEXT("Searching Session has Failed!"));
		return;
	}

	else
	{
		LOGWARNF(TEXT("Searching Session has Completed"));
		auto search_result = p_ReneSessionSearch->SearchResults;
		for (int32 i = 0 ; i < search_result.Num() ; i++)
		{
			FString str;
			search_result[i].Session.SessionSettings.Get(FName(TEXT("ROOMNAME")), str);
			OnFindReneSessionComplete.Broadcast(i, str);
			LOGWARNF(TEXT("%d 번 세션 : %s"), i, *str);
		}
	}
	
}

void URene_GameInstance::OnJoinReneSession(FName session_name, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Type::Success)
	{
		FString URL;
		
		if (p_ReneSessionInterface->GetResolvedConnectString(session_name, URL))
			if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
			{
				FInputModeGameOnly im;
				pc->SetInputMode(im);
				pc->SetShowMouseCursor(false);
				pc->ClientTravel(URL, TRAVEL_Absolute);
				LOGWARNF(TEXT("Joined URL : %s"), *URL);
			}
			else
				LOGERRORF(TEXT("Player Controller MIA"))
		else
			LOGERRORF(TEXT("URL has FuckedUp"))
	}
}
