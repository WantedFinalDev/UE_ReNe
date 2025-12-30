#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Rene_InterviewWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RENE_API URene_InterviewWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnEndButtonClicked();

public:
	/** "AI 생각 중..."과 같은 로딩 상태를 표시할 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> txt_Loading;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_End;

	/** AI의 자막을 표시할 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> txt_Subtitle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> txt_AISpeaking;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> txt_PlayerSpeaking;

	// New TextBlock for "Press to Talk" prompt
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> txt_PressToTalk;

public:
	/** 자막 텍스트를 갱신하고 화면에 표시합니다. */
	void UpdateSubtitle(const FString& NewSubtitle);

	/** 로딩 상태 UI의 활성화/비활성화 상태를 설정합니다. */
	void SetLoadingState(bool bIsLoading);

	/** 로딩 텍스트를 변경합니다. */
	void SetLoadingText(const FString& NewText);
	
	void SetInteractivity(bool bIsInteractive);
	
	void ShowAISpeaking(bool bIsSpeaking);
	
	void ShowPlayerSpeaking(bool bIsSpeaking);

	// Shows or hides the "Press to Talk" prompt
	void ShowPressToTalk(bool bShow);
};
