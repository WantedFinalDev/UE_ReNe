#include "Player/Rene_BasePlayer.h"
#include "UE_ReNe.h"

ARene_BasePlayer::ARene_BasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARene_BasePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARene_BasePlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ARene_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
