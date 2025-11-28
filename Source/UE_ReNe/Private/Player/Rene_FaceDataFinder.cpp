// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Rene_FaceDataFinder.h"


// Sets default values for this component's properties
URene_FaceDataFinder::URene_FaceDataFinder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URene_FaceDataFinder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URene_FaceDataFinder::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

