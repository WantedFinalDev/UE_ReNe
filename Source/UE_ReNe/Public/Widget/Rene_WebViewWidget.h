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
     * Sets the URL to be loaded by the web browser widget.
     * The actual loading happens in the Blueprint's Construct event.
     * @param NewURL The URL to load.
     */
    UFUNCTION(BlueprintCallable, Category = "Web View")
    void LoadURL(const FString& NewURL);

    UPROPERTY(BlueprintReadWrite, Category = "Web View")
    FString InitialURL;

protected:
    // The WebBrowser widget placed in the UMG designer.
    // Make sure to name the widget 'WebBrowser' in the UMG editor and check 'Is Variable'.
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    TObjectPtr<UWebBrowser> WebBrowser;
};
