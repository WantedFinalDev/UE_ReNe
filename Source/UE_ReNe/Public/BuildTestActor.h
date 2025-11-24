#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildTestActor.generated.h"

UCLASS()
class UE_RENE_API ABuildTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildTestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
