#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rene_Start_GameMode.generated.h"


UCLASS()
class UE_RENE_API ARene_Start_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	
	
private:
	
	
	
	
	
	/* Field */
public:
	/*
	 *	TODO : LobbyWidget을 WidgetSwitcher로 전환&로드
	 *	
	 */
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<class URene_StartWidget> StartUIClass;
	
	
	
private:
	/*UPROPERTY()
	class URene_StartWidget* StartUI;*/
	
	
	
};
