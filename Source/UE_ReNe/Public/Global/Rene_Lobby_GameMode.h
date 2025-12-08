#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rene_Lobby_GameMode.generated.h"


class URene_LobbyWidget;

UCLASS()
class UE_RENE_API ARene_Lobby_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	
	UFUNCTION(BlueprintCallable, Category = "UI")
	URene_LobbyWidget* GetLobbyUI() { return LobbyUI; }




private:
	
	
	
	
	/* Field */
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URene_LobbyWidget> LobbyUIClass;
	
	
	
private:
	UPROPERTY()
	class URene_LobbyWidget* LobbyUI;
	
	
	
};
