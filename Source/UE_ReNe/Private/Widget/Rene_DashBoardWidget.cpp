#include "Widget/Rene_DashBoardWidget.h"

#include "Widget/Rene_WebBrowser.h"

void URene_DashBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	
}

void URene_DashBoardWidget::SetURL(const FString& url)
{
	if (!url.IsEmpty())
	{
		URL = url;
		BP_Rene_WebBrowser->SetURL(URL);
	}
}
