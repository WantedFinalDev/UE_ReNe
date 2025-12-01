#pragma once

#include "CoreMinimal.h"
#include "UE_ReNePlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/VoiceInterface.h"
#include "Rene_PlayerController.generated.h"


UCLASS()
class UE_RENE_API ARene_PlayerController : public AUE_ReNePlayerController
{
	GENERATED_BODY()
	
	/* Method */
public:
	ARene_PlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateBoothUI();
	
	void CreateCompanyUI();
	void CreateSeekerUI();
	
	void OnPlayerListUpdated();
	TArray<TObjectPtr<class APlayerState>> GetAllPlayerState();
	UFUNCTION(Server, Reliable)
	void ServerRPC_TeleportWithTarget(APlayerState* targetstate, FVector targetlocation);
	
	/*	InputAction, InputMappingContext 바인드 필요
	 *	@ key pressed -> On xx UI() 호출 : UI on/off
	 */
	UFUNCTION(BlueprintCallable)
	void OnCompanyUI();
	UFUNCTION(BlueprintCallable)
	void OnSeekerUI();


private:
	void EnableUIControll();
	void DisableUIControll();
	TObjectPtr<class UUserWidget> GetUserWidget();
	
	
	/* Field */
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Company_Widget> companyui_class;
	
	// GM : PostLogin에서 미리 생성함. 사라지지않음.
	UPROPERTY()
	TObjectPtr<URene_Company_Widget> company_ui;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Seeker_Widget> seekerui_class;
	
	// GM : PostLogin에서 미리 생성함. 사라지지않음.
	UPROPERTY()
	TObjectPtr<URene_Seeker_Widget> seeker_ui;
	
//======voice======
public:
	/** PTT 시작 — 로컬에서 호출되어야 함 */
	UFUNCTION(BlueprintCallable, Category="Voice")
	void StartVoice();

	/** PTT 종료 — 로컬에서 호출되어야 함 */
	UFUNCTION(BlueprintCallable, Category="Voice")
	void StopVoice();

	/** 캐시된 VoiceInterface 포인터 (유효성 체크를 위해 사용) */
	IOnlineVoicePtr VoiceInterface;

	/** VoiceInterface 얻기 시도 (null-safe) */
	void EnsureVoiceInterface();

	/** 로컬 유저 인덱스 얻기 (로컬 플레이어 전용) */
	int32 GetLocalUserNum() const;

	
	
};
