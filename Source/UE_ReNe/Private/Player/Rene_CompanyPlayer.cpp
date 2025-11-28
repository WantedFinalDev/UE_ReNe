// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Rene_CompanyPlayer.h"


// Sets default values
ARene_CompanyPlayer::ARene_CompanyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARene_CompanyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARene_CompanyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARene_CompanyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

