#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_WebViewWidget.generated.h"

class UWebBrowser;

UCLASS()
class UE_RENE_API URene_WebViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    /**
     * Loads the specified URL in the web browser widget.
     * @param NewURL The URL to load.
     */
    UFUNCTION(BlueprintCallable, Category = "Web View")
    void LoadURL(const FString& NewURL);

protected:
    // The WebBrowser widget placed in the UMG designer.
    // Make sure to name the widget 'WebBrowser' in the UMG editor and check 'Is Variable'.
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWebBrowser> WebBrowser;
};
