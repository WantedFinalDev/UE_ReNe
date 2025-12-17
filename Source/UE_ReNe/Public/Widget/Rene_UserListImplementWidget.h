#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_UserListImplementWidget.generated.h"

class ARene_PlayerState;
class UTextBlock;
class UButton;

UCLASS()
class UE_RENE_API URene_UserListImplementWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;
	
	void SetUserImplementInfo(ARene_PlayerState* ps);
	
	
private:
	
	UFUNCTION()
	void OnClickedProfile();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Profile;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_UserName;


	
	
private:
	UPROPERTY()
	TObjectPtr<ARene_PlayerState> thisps;
	
	
};
