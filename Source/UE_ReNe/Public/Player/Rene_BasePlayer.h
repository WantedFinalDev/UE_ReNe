// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_ReNeCharacter.h"
#include "Rene_BasePlayer.generated.h"

UCLASS()
class UE_RENE_API ARene_BasePlayer : public AUE_ReNeCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARene_BasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
