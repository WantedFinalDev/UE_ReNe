// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UE_ReNeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUE_ReNeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person mesh, visible only to the owning player. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MouseLookAction;

public:
	AUE_ReNeCharacter();
	
protected:
	/** Called for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called for looking input */
	void LookInput(const FInputActionValue& Value);

	void DoAim(float Yaw, float Pitch);
	void DoMove(float Right, float Forward);
	void DoJumpStart();
	void DoJumpEnd();
			
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns FirstPersonMesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

// =================================================================
//                 걸어가서 앉기 기능 추가 (아래)
// =================================================================

public:
	// PlayerController로부터 호출될 함수
	void StandUp();
	void SetTargetSitTransform(const FTransform& NewTransform);

	// 서버에서만 실행될 이동 및 착석 관련 함수들 (public으로 변경)
	UFUNCTION(Server, Reliable)
	void Server_SetIsSitting(bool bNewSittingState);

protected:
	// Tick 및 복제 함수 오버라이드
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// '앉음' 상태가 클라이언트에 복제되었을 때 호출될 함수
	UFUNCTION()
	void OnRep_bIsSitting();

private:
	// 목표 지점 (클라이언트의 회전 동기화를 위해 복제)
	UPROPERTY(Replicated)
	FTransform TargetSitTransform;

	bool bIsMovingToSit;

	// '앉음' 상태를 나타내는 복제 변수
	UPROPERTY(ReplicatedUsing = OnRep_bIsSitting)
	bool bIsSitting;

	// [D] 부드러운 착석 보정을 위한 변수들
	bool bIsBlendingToSitTarget;
	float SitBlendDuration;
	float SitBlendElapsed;
	FVector SitBlendStartLoc;
	FRotator SitBlendStartRot;


// =================================================================
//                 걸어가서 앉기 기능 개선 (아래)
// =================================================================

public:
	// [B] 블루프린트에서 도착 판정 거리를 조절할 수 있도록 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SitArrivalThreshold;




// =================================================================
//                 속도 및 회전 개선용 변수 (아래)
// =================================================================
	
	// 원래의 최대 걷기 속도를 저장하기 위한 변수
	float OriginalMaxWalkSpeed;

	// 앉기 위해 회전 중인지 나타내는 플래그
	bool bIsTurningToSit;

	// =================================================================
	//                 카메라 이동 개선용 변수 (아래)
	// =================================================================

	// 서 있을 때의 카메라 상대 위치
	FVector StandingCameraLocation;
	// 목표 카메라 상대 위치
	FVector TargetCameraLocation;
	// 카메라 위치 보간 중인지 나타내는 플래그
	bool bIsInterpolatingCamera;

public:
	// 블루프린트에서 앉을 때의 회전 속도를 조절할 수 있도록 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SitTurnInterpSpeed;

	// 앉았을 때 카메라가 내려갈 상대적인 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector SittingCameraOffset;

	// 카메라 이동 보간 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CameraInterpSpeed;

public:
	// 블루프린트에서 앉음 상태를 안전하게 읽기 위한 Getter
	UFUNCTION(BlueprintPure, Category = "Character State")
	bool IsSitting() const { return bIsSitting; }
};
