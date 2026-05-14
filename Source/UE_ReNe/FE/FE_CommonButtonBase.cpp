#include "FE_CommonButtonBase.h"

#include "CommonTextBlock.h"

void UFE_CommonButtonBase::SetButtonText(const FText& InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		if (bUseUppercaseButtonText)
			CommonTextBlock_ButtonText->SetText(InText.ToUpper());
		else 
			CommonTextBlock_ButtonText->SetText(InText);
	}
}

void UFE_CommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetButtonText(ButtonDisplayText);
}
