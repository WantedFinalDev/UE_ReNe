#pragma once

#include "CoreMinimal.h"
#include "UE_ReNePlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/VoiceInterface.h"
#include "Voice/Rene_VoiceChatManager.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URene_VoiceChatManager* VoiceChatComponent;

	void StartVoicePress();
	void StopVoiceRelease();
	void EndPlay(EEndPlayReason::Type EndPlayReason);
};
