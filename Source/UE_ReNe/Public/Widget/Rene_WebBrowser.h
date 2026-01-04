#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "Rene_WebBrowser.generated.h"

UCLASS()
class UE_RENE_API URene_WebBrowser : public UWebBrowser
{
	GENERATED_BODY()
	
	/* Method */
public:
	// LoadURL 함수는 URene_WebViewWidget으로 이동됩니다.
	UFUNCTION(BlueprintCallable, Category = "URL")
	void SetURL(const FString& url);
	
private:
	
	
	
	
	
	/* Field */
public:
	
	
	
	
	
private:
	
	
	
	
	
	
};
