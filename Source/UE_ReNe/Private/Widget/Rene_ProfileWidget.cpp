#include "Widget/Rene_ProfileWidget.h"

#include "Components/Button.h"

void URene_ProfileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Upload->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickUpload);
	btn_DashBoard->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickDashBoard);
	btn_Return->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickReturn);
	
}

void URene_ProfileWidget::OnClickUpload()
{
	
}

void URene_ProfileWidget::OnClickDashBoard()
{
	//	OnClickDelDash.Broadcast();
	OnClickDashDynamic.Broadcast();
}

void URene_ProfileWidget::OnClickReturn()
{
	
}
