// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Rene_BasePlayer.h"


// Sets default values
ARene_BasePlayer::ARene_BasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARene_BasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARene_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARene_BasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

