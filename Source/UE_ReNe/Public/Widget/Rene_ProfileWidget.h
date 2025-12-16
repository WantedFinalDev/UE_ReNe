#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_ProfileWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

//	using FOnClickDocument = TMulticastDelegate<void()>;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickDocumnetDynamic);

UCLASS()
class UE_RENE_API URene_ProfileWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickDocument();
	UFUNCTION()
	void OnClickReport();
	UFUNCTION()
	void OnClickReturn();
	
	
	
	

private:
	
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> img_Face;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Document;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Report;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Return;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_UserName;
	
	//	FOnClickDocument OnClickDelDoc;
	
	UPROPERTY(BlueprintAssignable)
	FOnClickDocumnetDynamic OnClickDocDelDynamic;
	
	
	
private:
	
	
	
	
	
};
