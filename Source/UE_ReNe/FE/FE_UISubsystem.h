#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FE_UISubsystem.generated.h"

class UWidget_ActivatableBase;
class UWidget_PrimaryLayout;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

UCLASS()
class UE_RENE_API UFE_UISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	/* Method */
public:
	static UFE_UISubsystem* GetUISubsystem(const UObject* WorldContextObject);
	
	//~ Begin USubsytem.h
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsystem.h

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InCreatedWidget);
	
	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AsyncPushStateCallback);
	
private:
	
	
	/* Field */
public:


private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_PrimaryLayout> CreatedPrimaryLayout;
};
