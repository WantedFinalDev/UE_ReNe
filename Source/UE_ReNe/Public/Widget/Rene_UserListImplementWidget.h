#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_UserListImplementWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class UE_RENE_API URene_UserListImplementWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	void SetUserImplementInfo(APlayerState* ps);
	
	
	
	
private:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickedP2P();
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_P2P;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> txt_UserName;
	
	
	
	
	
private:
	UPROPERTY()
	TObjectPtr<APlayerState> playerstate;
	
	
	
};
