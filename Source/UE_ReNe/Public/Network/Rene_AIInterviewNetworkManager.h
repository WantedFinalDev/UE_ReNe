#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_AIInterviewNetworkManager.generated.h"

class ARene_PlayerController;
class ARene_AI_Interviewer;

// 대기 중인 요청 데이터 구조체
struct FPendingAIInterviewData
{
	FString StartURL;
	int32 UserID;
	int32 CompanyID;
	int32 JobGroupID;
	TWeakObjectPtr<ARene_AI_Interviewer> TargetInterviewer; // 약한 참조로 안전하게 보관
	bool bIsValid = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RENE_API URene_AIInterviewNetworkManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	URene_AIInterviewNetworkManager();

	// 데이터 캐싱
	void CacheInterviewRequest(const FString& URL, int32 UserID, int32 CompanyID, int32 JobGroupID, ARene_AI_Interviewer* Interviewer);

	// 실제 전송 (이동 완료 후 호출)
	void SendCachedRequest();

private:
	void OnAIInterviewStartResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FPendingAIInterviewData PendingData;
};
