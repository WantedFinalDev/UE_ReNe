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

	// [추가] URL을 GameInstance 설정에 맞춰 새로고침하는 함수
	UFUNCTION(BlueprintCallable)
	void RefreshDashboardURL();
	
	
	
	private:
	
	
	
	
	
	/* Field */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=URL)
	FString URL;
	
	UPROPERTY(meta=(BindWidget))
	URene_WebBrowser* BP_Rene_WebBrowser;
	
private:
	
	
	
	
	
};
