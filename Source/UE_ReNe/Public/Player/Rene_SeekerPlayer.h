// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rene_BasePlayer.h"
#include "Rene_SeekerPlayer.generated.h"

UCLASS()
class UE_RENE_API ARene_SeekerPlayer : public ARene_BasePlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARene_SeekerPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
