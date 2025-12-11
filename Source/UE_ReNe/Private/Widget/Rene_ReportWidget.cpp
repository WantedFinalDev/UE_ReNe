#include "Widget/Rene_ReportWidget.h"

#include "Components/Button.h"
#include "Network/Rene_FileFetcher.h"
#include "Engine/Texture2D.h"
#include "Logging/LogMacros.h"

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
		FileFetcher->FetchFile(PngUrl);
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
