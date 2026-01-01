#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_Seeker_Widget.generated.h"

class URene_DashBoardWidget;
class URene_WebViewWidget;
class ARene_PlayerController;
class URene_ProfileWidget;
class URene_ReportWidget;
class UCanvasPanel;
class UWidgetSwitcher;
class UButton;

UCLASS()
class UE_RENE_API URene_Seeker_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;


private:
	UFUNCTION()
	void OnClickedClose();
	
	UFUNCTION()
	void OnClickedReturn();
	
	UFUNCTION()
	void OnClickedDash();
	
	UFUNCTION()
	void OnClickedDashToMain();


	/* Field */
public:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> sw_Main;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> cv_Main;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> cv_Dash;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Close;		//	UI Close
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_DashToMain;
	
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_ProfileWidget> WBP_ProfileUI;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_DashBoardWidget> WBP_DashBoardUI;
	
private:
	UPROPERTY()
	TObjectPtr<ARene_PlayerController> pc;


};