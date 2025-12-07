#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_Company_Widget.generated.h"


class UWidgetSwitcher;
class USizeBox;
class UScrollBox;
class UButton;

UCLASS()
class UE_RENE_API URene_Company_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	
	
	

private:
	UFUNCTION()
	void OnClickedClose();
	UFUNCTION()
	void OnClickedList();
	UFUNCTION()
	void OnClickedMainToReport();
	UFUNCTION()
	void OnClickedReportToMain();
	
	void PopulateUserList();
	void ClearUserList();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> sw_Switcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_MainClose;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_UserList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> scr_UserList;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_MainToReport;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_ReportToMain;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<class URene_UserListImplementWidget> ImplementWidget;

	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UUserWidget> ResultWidget;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> p_ResultUI;
	
	//	Private Room : Placed TargetPoint Actor
	//	Vector 좌표 고정으로 대체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetOfTeleport;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> size_Result;
	
private:
	bool bIsVisibleList = false;
	
	// 별도 배열공간 필요시 추가
	// TArray<URene_UserListImplement*> arr_userlistwidget;
	
	
};
