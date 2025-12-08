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
	void SetUserImplementInfo(ARene_PlayerState* ps);
	void SetTeleportLocation(FVector v);
	
	
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
	
	UPROPERTY(EditAnywhere)
	FVector TeleportLocation;

	
	
private:
	UPROPERTY()
	TObjectPtr<ARene_PlayerState> thisps;
	
	
};
