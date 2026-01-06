#include "Widget/Rene_WebViewWidget.h"
#include "WebBrowser.h"

void URene_WebViewWidget::LoadURL(const FString& NewURL)
{
    InitialURL = NewURL;
}
