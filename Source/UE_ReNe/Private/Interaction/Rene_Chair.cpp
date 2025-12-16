#include "Interaction//Rene_Chair.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UE_ReNeCharacter.h" // AUE_ReNeCharacter 헤더 포함

// Sets default values
ARene_Chair::ARene_Chair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // 의자는 매 프레임 Tick이 필요 없을 수 있습니다.

	// 루트 컴포넌트 생성
	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh"));
	RootComponent = ChairMesh;

	// 앉을 위치 컴포넌트 생성 및 ChairMesh에 부착
	SitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SitLocation"));
	SitLocation->SetupAttachment(ChairMesh);
	// SitLocation의 상대 위치와 회전을 조정하여 캐릭터가 의자에 앉을 정확한 위치를 지정합니다.
	// 이 값은 의자 모델에 따라 블루프린트에서 조정하는 것이 좋습니다.
	SitLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // 예시: 의자 위 50cm
	SitLocation->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f)); // 예시: 의자 반대 방향을 바라보도록

}

// Called when the game starts or when spawned
void ARene_Chair::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARene_Chair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARene_Chair::Interact_Implementation(AUE_ReNeCharacter* InteractingCharacter)
{
	if (InteractingCharacter && SitLocation)
	{
		// 캐릭터에게 SitLocation의 Transform으로 앉으라고 지시합니다.
		InteractingCharacter->SetTargetSitTransform(SitLocation->GetComponentTransform());
	}
}
