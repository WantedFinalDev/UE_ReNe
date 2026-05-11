#include "AsyncAction_PushSoftWidget.h"
#include "FE/FE_UISubsystem.h"
#include "FE/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
	APlayerController* OwningPlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
	UPARAM(meta = (Categories = "FE.WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack wass passed a null widget class"));
	
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->RegisterWithGameInstance(World);
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->bCachedFocusOnNewPushedWidget = bFocusOnNewPushedWidget;
			return Node;
		}
	}
	
	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	Super::Activate();
	
	UFE_UISubsystem* UISubsystem = UFE_UISubsystem::GetUISubsystem(CachedOwningWorld.Get());
	UISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass, 
		[this](EAsyncPushWidgetState InState, UWidget_ActivatableBase* PushedWidget)
		{
			switch (InState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				break;
				
			case EAsyncPushWidgetState::AfterPush:
				OnAfterPush.Broadcast(PushedWidget);
				if (bCachedFocusOnNewPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusWidget())
					{
						WidgetToFocus->SetFocus();
					}
				}
				SetReadyToDestroy();
				break;
				
				default:
				break;
			}
		});
	
}
