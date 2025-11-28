#pragma once

#include "CoreMinimal.h"
#include "UE_ReNePlayerController.h"
#include "Rene_PlayerController.generated.h"


UCLASS()
class UE_RENE_API ARene_PlayerController : public AUE_ReNePlayerController
{
	GENERATED_BODY()
	
	/* Method */
public:
	ARene_PlayerController();
	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateBoothUI();
	
	void CreateCompanyUI();
	void CreateSeekerUI();

private:
	void EnableUIControll();
	void DisableUIControll();
	TObjectPtr<class UUserWidget> GetUserWidget();
	
	
	/* Field */
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Company_Widget> companyui_class;
	
	UPROPERTY()
	TObjectPtr<URene_Company_Widget> company_ui;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_Seeker_Widget> seekerui_class;
	
	UPROPERTY()
	TObjectPtr<URene_Seeker_Widget> seeker_ui;
	
	
	
	
	
private:
	
	
	
	
	
};
