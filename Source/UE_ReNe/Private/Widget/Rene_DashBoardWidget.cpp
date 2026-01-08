#include "Widget/Rene_DashBoardWidget.h"
#include "Widget/Rene_WebBrowser.h"
#include "Global/Rene_GameInstance.h" // 헤더 추가

void URene_DashBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshDashboardURL();
}

void URene_DashBoardWidget::RefreshDashboardURL()
{
	if (URene_GameInstance* GameInstance = GetGameInstance<URene_GameInstance>())
	{
		const FString& BaseEndpoint = GameInstance->GetNetworkSettings().DashboardEndpoint;
		const FString& UserID = GameInstance->GetCachedUserData().ID;

		if (!BaseEndpoint.IsEmpty() && !UserID.IsEmpty())
		{
			// Case B: Endpoint에 이미 쿼리 파라미터 키가 포함되어 있다고 가정하고 단순 결합
			FString FinalURL = BaseEndpoint + UserID;
			
			SetURL(FinalURL);
			UE_LOG(LogTemp, Log, TEXT("DashBoardWidget: Auto-loaded URL: %s"), *FinalURL);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DashBoardWidget: Failed to load URL. Endpoint: %s, UserID: %s"), *BaseEndpoint, *UserID);
		}
	}
}

void URene_DashBoardWidget::SetURL(const FString& url)
{
	if (!url.IsEmpty())
	{
		URL = url;
		
		FString RoleStr = TEXT("Unknown");
		if (GetOwningPlayer())
		{
			RoleStr = GetOwningPlayer()->HasAuthority() ? TEXT("Server") : TEXT("Client");
		}
		UE_LOG(LogTemp, Warning, TEXT("[%s] DASHBOARD DEBUG: SetURL Called with: %s"), *RoleStr, *URL);
		
		if (BP_Rene_WebBrowser)
		{
			BP_Rene_WebBrowser->SetURL(URL);
		}
	}
}
