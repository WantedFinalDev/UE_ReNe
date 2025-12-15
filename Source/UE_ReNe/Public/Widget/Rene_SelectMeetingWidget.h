#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Rene_SelectMeetingWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RENE_API URene_SelectMeetingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 레벨 블루프린트에서 호출할 함수
	UFUNCTION(BlueprintCallable)
	void SetTargetActors(AActor* InPrivateInterviewTarget, AActor* InAIInterviewTarget);

protected:
	virtual void NativeConstruct() override;

private:
	// 버튼 클릭 시 호출될 함수들
	UFUNCTION()
	void OnStartPrivateInterviewClicked();

	UFUNCTION()
	void OnStartAIInterviewClicked();

protected:
	// 위젯 블루프린트의 버튼과 연결될 변수들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_StartPrivateInterview;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_StartAIInterview;

private:
	// 타겟 액터를 저장할 내부 변수 (블루프린트 노출 없음)
	UPROPERTY()
	TObjectPtr<AActor> PrivateInterviewTargetActor;

	UPROPERTY()
	TObjectPtr<AActor> AIInterviewTargetActor;
};
