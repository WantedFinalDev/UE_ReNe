#include "Widget/Rene_ProfileWidget.h"

#include "Components/Button.h"

void URene_ProfileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Document->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickDocument);
	btn_Report->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickReport);
	btn_Return->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickReturn);
	
}

void URene_ProfileWidget::OnClickDocument()
{
	OnClickDelDoc.Broadcast();
}

void URene_ProfileWidget::OnClickReport()
{
	
}

void URene_ProfileWidget::OnClickReturn()
{
	
}
