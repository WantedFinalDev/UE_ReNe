#include "Widget/Rene_WebBrowser.h"

// 내용은 URene_WebViewWidget.cpp 로 이동됩니다.
void URene_WebBrowser::SetURL(const FString& url)
{
	if (!url.IsEmpty())
		InitialURL = url;
}
