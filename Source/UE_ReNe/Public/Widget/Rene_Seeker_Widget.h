#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rene_Seeker_Widget.generated.h"

/*
 * [이미지 뷰어 기능 흐름 요약]
 *
 * 1. 사용자가 조회 버튼(btn_Search) 클릭
 *    └─> OnClickedSearch() 호출
 *
 * 2. HTTP 요청 시작
 *    └─> RequestImageFromServer() 함수에서 FHttpModule 사용
 *    └─> 서버 URL로 GET 요청 전송 (FileFetcher 패턴 참고)
 *
 * 3. 비동기 콜백 처리
 *    └─> OnImageDownloadComplete() 콜백 함수 실행
 *    └─> 응답 검증: HTTP 상태 코드, 데이터 유효성 체크
 *
 * 4. 이미지 변환
 *    └─> IImageWrapperModule로 PNG/JPG 포맷 감지
 *    └─> 바이너리 데이터 → UTexture2D 변환
 *
 * 5. UI 업데이트
 *    └─> UImage 위젯에 Texture2D 설정
 *    └─> 이미지 표시 영역(img_Viewer)에 렌더링
 *
 * 6. 에러 처리
 *    └─> 네트워크 실패, 잘못된 이미지 포맷 등 예외 상황 로깅
 */

class UButton;
class UImage;
class IHttpRequest;
class IHttpResponse;

UCLASS()
class UE_RENE_API URene_Seeker_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void NativeConstruct() override;



private:
	// TODO(human): 기존 Close 버튼 핸들러
	UFUNCTION()
	void OnClickedClose();

	// TODO(human): 조회 버튼 클릭 시 호출되는 핸들러
	// - 서버로부터 이미지 다운로드 시작
	// - RequestImageFromServer() 호출
	UFUNCTION()
	void OnClickedSearch();

	// TODO(human): HTTP 요청 시작 함수
	// - FHttpModule::Get()으로 HTTP 모듈 획득
	// - CreateRequest()로 요청 객체 생성
	// - SetURL(), SetVerb("GET") 설정
	// - OnProcessRequestComplete() 콜백 바인딩
	// - ProcessRequest() 호출
	void RequestImageFromServer();

	// TODO(human): HTTP 요청 완료 콜백 (비동기)
	// - 파라미터: FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful
	// - Response 유효성 검증 (IsValid, HTTP 상태 코드)
	// - Response->GetContent()로 바이너리 데이터 획득
	// - IImageWrapperModule로 이미지 포맷 감지 (PNG/JPG)
	// - UTexture2D 생성 및 픽셀 데이터 복사
	// - img_Viewer->SetBrushFromTexture() 호출
	void OnImageDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	/* Field */
public:
	// TODO(human): Blueprint에서 바인딩할 위젯들
	// WBP_Seeker_Widget에서 같은 이름으로 만들어야 함

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Close;

	// TODO(human): 조회 버튼
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Search;

	// TODO(human): 이미지를 표시할 위젯
	// - UImage는 Brush 속성에 Texture2D를 설정할 수 있음
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> img_Viewer;

	// TODO(human): 서버 URL 설정 (Blueprint에서 수정 가능)
	// - EditAnywhere: 블루프린트 에디터에서 수정 가능
	// - 테스트용 기본값: "http://100.112.161.59:8000/api/v1/..."
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Seeker|Network")
	FString ServerImageURL;



private:
	// TODO(human): 다운로드된 Texture2D 캐싱용 (선택사항)
	// - 같은 이미지 재요청 시 재사용
	UPROPERTY()
	TObjectPtr<UTexture2D> CachedTexture;





};

/*
 * ============================================================================
 * [최우선 TODO - Blueprint 작업 필요]
 * ============================================================================
 *
 * 1. WBP_Seeker_Widget.uasset 블루프린트를 열기
 *    위치: Content/UI/WBP_Seeker_Widget.uasset
 *
 * 2. 다음 위젯들을 추가하고 반드시 같은 이름으로 설정:
 *    - Button 위젯 추가 → 이름을 "btn_Search"로 설정
 *    - Image 위젯 추가 → 이름을 "img_Viewer"로 설정
 *
 * 3. 레이아웃 예시:
 *    ┌─────────────────────────────────┐
 *    │  [btn_Close]      [btn_Search]  │  ← 상단에 버튼들
 *    ├─────────────────────────────────┤
 *    │                                 │
 *    │         img_Viewer              │  ← 중앙에 이미지 표시 영역
 *    │      (이미지 표시 공간)          │
 *    │                                 │
 *    └─────────────────────────────────┘
 *
 * 4. img_Viewer 권장 설정:
 *    - Size: Fill 또는 원하는 크기
 *    - Brush → Tiling: NoTile
 *
 * 5. 블루프린트 저장 후 C++ 코드 구현 시작
 *
 * ============================================================================
 */
