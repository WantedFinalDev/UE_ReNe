#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_LobbyWidget.generated.h"


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
	
	UFUNCTION()
	void OnClickedCloseList();
	
	UFUNCTION()
	void OnClickedFind();
	
	
	
	
	
	/* Field */
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Find;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_CloseList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> scr_SessionList;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_SessionInfoWidget> session_info_widget;
	
	
	
private:
	
	
	
	
};
