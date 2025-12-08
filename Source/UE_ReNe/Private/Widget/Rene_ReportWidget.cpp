#include "Widget/Rene_ReportWidget.h"

#include "Components/Button.h"

void URene_ReportWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_EnterUpload->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickEnterUpld);
	btn_EnterDownload->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickEnterDownld);
	btn_Upload->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickUpload);
	btn_Download->OnClicked.AddDynamic(this, &URene_ReportWidget::OnClickDownload);
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
	
}
