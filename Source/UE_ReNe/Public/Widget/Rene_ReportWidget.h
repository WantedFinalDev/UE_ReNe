#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/IHttpRequest.h"
#include "Rene_ReportWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UCanvasPanel;
class UTexture2D;
class UImage;
class IHttpRequest;
class IHttpResponse;
class URene_FileFetcher;

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

UCLASS()
class UE_RENE_API URene_ReportWidget : public UUserWidget
{
	GENERATED_BODY()

	/* Method */
public:
	virtual void NativeConstruct() override;

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
	void OnImageDownloadComplete(FHttpRequestPtr request, FHttpResponsePtr response, bool bsuccess);
	
	UFUNCTION()
	void OnClickEnterUpld();
	UFUNCTION()
	void OnClickEnterDownld();
	UFUNCTION()
	void OnClickUpload();
	UFUNCTION()
	void OnClickDownload();
	
private:
	void OnFileFetchCompleted(UTexture2D* FetchedTexture);
	
	
	
	/* Field */
public:
	///////////////* UI *////////////////
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_EnterDownload;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_EnterUpload;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Download;	//	Doc 다운로드
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btn_Upload;		//	Doc 업로드
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> sw_Report;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> cv_Up;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> cv_Down;
	//////////////////////////////////////
	
	// TODO(human): 이미지를 표시할 위젯
	// - UImage는 Brush 속성에 Texture2D를 설정할 수 있음

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

	UPROPERTY()
	TObjectPtr<URene_FileFetcher> FileFetcher;
};
