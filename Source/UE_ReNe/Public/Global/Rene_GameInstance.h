#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Rene_GameInstance.generated.h"


UCLASS()
class UE_RENE_API URene_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	/* Method */
public:
	virtual void Init() override;

	
	
private:
	
	
	
	
	
	/* Field */
public:
	IOnlineSessionPtr Session;
	
	
	
	
	
private:
	
	
	
	
};
