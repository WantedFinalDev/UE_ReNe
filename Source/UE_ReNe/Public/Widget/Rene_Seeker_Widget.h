#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_Seeker_Widget.generated.h"

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
	void OnClickReturn();


	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Close;		//	UI Close
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_EnterReport;	//	Open Report UI
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> sw_Main;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> cv_Main;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> cv_Report;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_ReportWidget> WBP_ReportWidget;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_ProfileWidget> WBP_ProfileUI;
	
private:



};