#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Interfaces/IHttpRequest.h" // Required for FHttpRequestPtr
#include "Interfaces/IHttpResponse.h" // Required for FHttpResponsePtr
#include "Rene_SelectMeetingWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectMeetingOnClickedBack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterviewClicked);


class ARene_AI_Interviewer; // Forward declaration for the actual AI interviewer actor

/**
 * 
 */
UCLASS()
class UE_RENE_API URene_SelectMeetingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 레벨 블루프린트에서 호출할 함수 (플레이어 이동 타겟)
	UFUNCTION(BlueprintCallable)
	void SetTargetActors(AActor* InPrivateInterviewTarget, AActor* InAIInterviewTarget);

	// 레벨 블루프린트에서 호출할 함수 (실제 AI 면접관 액터)
	UFUNCTION(BlueprintCallable)
	void SetActualAIInterviewer(ARene_AI_Interviewer* InAIInterviewer);

	// [추가] 면접 대상 기업 및 직군 ID 설정 (Blueprint에서 호출 필수)
	UFUNCTION(BlueprintCallable, Category = "Rene|Interview")
	void SetInterviewDetails(int32 InCompanyID, int32 InJobGroupID);
	
	UFUNCTION(BlueprintCallable)
	void OnClickedBackButton();

protected:
	virtual void NativeConstruct() override;

private:
	// 버튼 클릭 시 호출될 함수들
	UFUNCTION()
	void OnStartPrivateInterviewClicked();

	UFUNCTION()
	void OnStartAIInterviewClicked();

	UFUNCTION()
	void SwitchToPrivateInterviewCamera_Delayed();

	// Callback for the AI Interview Start HTTP request
	void OnAIInterviewStartResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
	// 위젯 블루프린트의 버튼과 연결될 변수들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_StartPrivateInterview;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_StartAIInterview;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> btn_Back;

private:
	// 타겟 액터를 저장할 내부 변수 (블루프린트 노출 없음) - 플레이어 이동 위치
	UPROPERTY()
	TObjectPtr<AActor> PrivateInterviewTargetActor;

	UPROPERTY()
	TObjectPtr<AActor> AIInterviewTargetActor; // This is the player's sitting target, not the AI interviewer itself

	// 실제 AI 면접관 액터 (음성 재생 등)
	UPROPERTY()
	TObjectPtr<ARene_AI_Interviewer> ActualAIInterviewer;

	// [추가] 면접 요청에 사용할 대상 ID (기본값 0)
	int32 TargetCompanyID = 0;
	int32 TargetJobGroupID = 0;
	
	UPROPERTY(BlueprintAssignable)
	FSelectMeetingOnClickedBack OnClickedBack;
	
	UPROPERTY(BlueprintAssignable)
	FInterviewClicked OnClickedInterview;
	
	
};
