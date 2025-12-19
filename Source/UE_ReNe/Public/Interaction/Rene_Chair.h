#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Rene_InteractableInterface.h" // 인터페이스 헤더 포함
#include "Rene_Chair.generated.h"

UCLASS()
class UE_RENE_API ARene_Chair : public AActor, public IRene_InteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARene_Chair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IRene_InteractableInterface 인터페이스 함수 구현
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AUE_ReNeCharacter* InteractingCharacter);
	virtual void Interact_Implementation(AUE_ReNeCharacter* InteractingCharacter) override;

protected:
	/** 캐릭터가 앉을 위치와 회전을 정의하는 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chair")
	USceneComponent* SitLocation;

	/** 의자의 스태틱 메시 컴포넌트입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chair")
	UStaticMeshComponent* ChairMesh;
};
