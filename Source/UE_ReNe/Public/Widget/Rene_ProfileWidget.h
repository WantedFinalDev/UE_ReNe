#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_ProfileWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

//	using FOnClickDocument = TMulticastDelegate<void()>;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickDashBoardDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickReturnDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickUploadDynamic);

UCLASS()
class UE_RENE_API URene_ProfileWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickUpload();
	UFUNCTION()
	void OnClickDashBoard();
	UFUNCTION()
	void OnClickReturn();
	UFUNCTION()
	void SetProfileName(bool bSuccess, const FReneUserData& UserData, FString ErrorMessage);
	
	
	

private:
	
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> img_Face;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Upload;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_DashBoard;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Return;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_UserName;
	
	//	FOnClickDocument OnClickDelDoc;
	
	UPROPERTY(BlueprintAssignable)
	FOnClickDashBoardDynamic OnClickDashDynamic;
	
	UPROPERTY(BlueprintAssignable)
	FOnClickReturnDynamic OnClickReturnDynamic;
	
	UPROPERTY(BlueprintAssignable)
	FOnClickUploadDynamic OnClickUploadDynamic;
	
private:
	
	
	
	
	
};
