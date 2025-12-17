#include "Widget/Rene_ReportWidget.h"

#include "Components/Button.h"
#include "Network/Rene_FileFetcher.h"
#include "Engine/Texture2D.h"
#include "Logging/LogMacros.h"
#include "Global/Rene_GameInstance.h" // GameInstance 헤더 추가

void URene_ReportWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_EnterUpload->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickEnterUpld);
	btn_EnterDownload->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickEnterDownld);
	btn_Upload->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickUpload);
	btn_Download->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickDownload);

	FileFetcher = NewObject<URene_FileFetcher>(this);
	FileFetcher->OnFileFetchSuccess.BindUObject(this, &URene_ReportWidget::OnFileFetchCompleted);
}

void URene_ReportWidget::RequestImageFromServer()
{
	
}

void URene_ReportWidget::OnImageDownloadComplete(FHttpRequestPtr request, FHttpResponsePtr response, bool bsuccess)
{
	
}

void URene_ReportWidget::OnClickEnterUpld()
{
	
}

void URene_ReportWidget::OnClickEnterDownld()
{
	
}

void URene_ReportWidget::OnClickUpload()
{
	
}

void URene_ReportWidget::OnClickDownload()
{
	if (FileFetcher)
	{
		// GameInstance에서 네트워크 설정 가져오기
		URene_GameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<URene_GameInstance>() : nullptr;
		if (GameInstance)
		{
			const FString ReportURL = GameInstance->GetNetworkSettings().ReportImageDownloadURL;
			if (!ReportURL.IsEmpty())
			{
				FileFetcher->FetchFile(ReportURL);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ReportImageDownloadURL is not set in DataTable."));
			}
		}
	}
}

void URene_ReportWidget::OnFileFetchCompleted(UTexture2D* FetchedTexture)
{
	if (FetchedTexture)
	{
		UE_LOG(LogTemp, Log, TEXT("File fetch successful. Texture created."));
		CachedTexture = FetchedTexture;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("File fetch failed. Texture is invalid."));
	}
}
