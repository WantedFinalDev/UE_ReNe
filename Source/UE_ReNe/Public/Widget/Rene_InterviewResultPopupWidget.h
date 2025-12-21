#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_InterviewResultPopupWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowReportClicked);

UCLASS()
class UE_RENE_API URene_InterviewResultPopupWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    /** Sets the interview result ID to be displayed on the widget. */
    UFUNCTION(BlueprintCallable, Category = "AI Interview")
    void SetResultID(int32 InResultID);

    /** Delegate that is broadcast when the 'Show Report' button is clicked. */
    UPROPERTY(BlueprintAssignable, Category = "AI Interview|Events")
    FOnShowReportClicked OnShowReportClicked;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> txt_ResultID;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> btn_ShowReport;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> btn_Close;

private:
    UFUNCTION()
    void HandleShowReportClicked();

    UFUNCTION()
    void HandleCloseClicked();
};
