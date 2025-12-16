#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Rene_NPCBase.generated.h"

UCLASS()
class UE_RENE_API ARene_NPCBase : public APawn
{
	GENERATED_BODY()

public:
	ARene_NPCBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	private:
	
	
	
	
	
	
	/* Field */
public:
	
	
	
	
	
	
private:
	
	
	
	
	

};
