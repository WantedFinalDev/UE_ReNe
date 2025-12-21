#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_LobbyWidget.generated.h"


class URene_ReportWidget;
class URene_DashBoardWidget;
class URene_ProfileWidget;
class UEditableTextBox;
class UWidgetSwitcher;
class UScrollBox;
class UButton;

UCLASS()
class UE_RENE_API URene_LobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	
	
private:
	UFUNCTION()
	void OnFindComplete(int idx, FString str);
	
	//	12.10 수 비둘기
	//	Start+Lobby 통폐합
	// UFUNCTION()
	// void OnClickedCloseList();
	
	UFUNCTION()
	void OnClickedFind();
	
	UFUNCTION()
	void OnClickedCreate();
	
	UFUNCTION()
	void OnClickedCreateSession();
	
	UFUNCTION()
	void OnClickedBack();
	
	UFUNCTION()
	void OnClickedDash();
	
	UFUNCTION()
	void OnClickedReturnMain();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Find;
	
	//	12.10 수 비둘기
	//	Start+Lobby 통폐합
	// UPROPERTY(meta=(BindWidget))
	// TObjectPtr<UButton> btn_CloseList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Create;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_CreateSession;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_BackToMain;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> scr_SessionList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_SessionName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> etxt_MaxPlayer;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_SessionInfoWidget> session_info_widget;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_ProfileWidget> WBP_ProfileUI_main;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_ProfileWidget> WBP_ProfileUI_dash;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URene_DashBoardWidget> WBP_DashBoardUI;
	
	
	
private:
	
	
	
	
};
