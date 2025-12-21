#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
// #include "UE_ReNeCharacter.h" // 순환 참조를 피하기 위해 제거
#include "Rene_InteractableInterface.generated.h"

// AUE_ReNeCharacter에 대한 전방 선언
class AUE_ReNeCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URene_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief 게임 내 상호작용 가능한 객체들이 구현할 인터페이스입니다.
 */
class UE_RENE_API IRene_InteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief 캐릭터가 이 객체와 상호작용할 때 호출됩니다.
	 * @param InteractingCharacter 상호작용을 시도하는 캐릭터입니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AUE_ReNeCharacter* InteractingCharacter);
};
