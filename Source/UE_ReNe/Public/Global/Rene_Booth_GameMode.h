#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rene_Booth_GameMode.generated.h"


UCLASS()
class UE_RENE_API ARene_Booth_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	
	
private:
	
	
	
	
	
	/* Field */
public:
	
	
	
	
	
private:
	
	
	
	
	
};
