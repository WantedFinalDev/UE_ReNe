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
	//	12.10 수 16:49 비둘기
	//	StartUI LobbyUI 통폐합으로 LobbyMap 삭제됨.
	//	LobbyMap 관련 삭제로 해당 GM과 Lobby Widget은 StartGameMode로 이관되었음.
	
	
	// virtual void BeginPlay() override;
	// virtual void PostLogin(APlayerController* NewPlayer) override;
	//
	// UFUNCTION(blueprintCallable, Category = "UI")
	// void DisplayLobbyUI(APlayerController* pc);
	//
	// UFUNCTION(blueprintCallable, Category = "UI")
	// void SwitchDisplayUI();

private:
	
	
	
	
	/* Field */
public:
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<class URene_LobbyWidget> LobbyUIClass;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	// class URene_LobbyWidget* LobbyUI;
	
	
private:
	
	
	
};
