#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_DashBoardWidget.generated.h"

class URene_WebBrowser;

UCLASS()
class UE_RENE_API URene_DashBoardWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
	public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetURL(const FString& url);
	
	
	
	private:
	
	
	
	
	
	/* Field */
public:
	FString URL;
	
	UPROPERTY(meta=(BindWidget))
	URene_WebBrowser* BP_Rene_WebBrowser;
	
private:
	
	
	
	
	
};
