#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Rene_BasePlayer.generated.h"

UCLASS()
class UE_RENE_API ARene_BasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ARene_BasePlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
