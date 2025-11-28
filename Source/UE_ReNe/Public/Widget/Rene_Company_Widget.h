#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_Company_Widget.generated.h"


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
	
	void PopulateUserList();
	void ClearUserList();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Close;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_UserList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> scr_UserList;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<class URene_UserListImplementWidget> ImplementWidget;
	
private:
	bool bIsVisibleList = false;
	
	// 별도 배열공간 필요시 추가
	// TArray<URene_UserListImplement*> arr_userlistwidget;
	
	
};
