#include "Widget/Rene_Booth_Widget.h"
#include "UE_ReNe.h"
#include "Components/Button.h"

void URene_Booth_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Noti->OnClicked.AddDynamic(this, &URene_Booth_Widget::OnClickedNoti);
}

void URene_Booth_Widget::OnClickedNoti()
{
	SHOWWARN()
}
