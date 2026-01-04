// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_ReNeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UE_ReNe.h"
#include "Player/Rene_PlayerController.h"

AUE_ReNeCharacter::AUE_ReNeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// --- '걸어가서 앉기' 관련 초기화 ---
	PrimaryActorTick.bCanEverTick = true; // Tick 함수를 사용하기 위해 활성화
	bIsMovingToSit = false;
	bIsSitting = false;

	// [B] 도착 판정 거리 기본값 설정
	SitArrivalThreshold = 50.0f; // 50cm

	// [D] 부드러운 착석 보정 변수 초기화
	bIsBlendingToSitTarget = false;
	SitBlendDuration = 0.25f;
	SitBlendElapsed = 0.f;

	// --- 속도 및 회전 개선용 변수 초기화 ---
	OriginalMaxWalkSpeed = 0.0f;
	bIsTurningToSit = false;
	SitTurnInterpSpeed = 5.0f; // 기본 회전 속도 설정

	// --- 카메라 이동 개선용 변수 초기화 ---
	bIsInterpolatingCamera = false;
	SittingCameraOffset = FVector(0.f, 0.f, -40.f); // 기본값: Z축으로 40cm 내려감
	CameraInterpSpeed = 7.0f; // 기본 카메라 이동 속도
}

void AUE_ReNeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// =================================================================
	//                 원래 카메라 위치 저장 (아래)
	// =================================================================
	if (FirstPersonCameraComponent)
	{
		StandingCameraLocation = FirstPersonCameraComponent->GetRelativeLocation();
	}
	// =================================================================

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AUE_ReNeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUE_ReNeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUE_ReNeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE_ReNeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE_ReNeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AUE_ReNeCharacter::LookInput);
	}
	else
	{
		UE_LOG(Rene, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AUE_ReNeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AUE_ReNeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AUE_ReNeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AUE_ReNeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AUE_ReNeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AUE_ReNeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}


// =================================================================
//                 걸어가서 앉기 기능 추가 (아래)
// =================================================================

void AUE_ReNeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUE_ReNeCharacter, bIsSitting);
	DOREPLIFETIME(AUE_ReNeCharacter, TargetSitTransform);
}

void AUE_ReNeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		// [C] 이동 중 도착 판정 (2D 거리 사용)
		if (bIsMovingToSit)
		{
			const float DistanceToTarget2D = FVector::Dist2D(GetActorLocation(), TargetSitTransform.GetLocation());

			// [C] Z를 무시한 2D 거리가 임계값보다 작고, 속도가 거의 0이면 도착으로 간주
			if (DistanceToTarget2D < SitArrivalThreshold && GetVelocity().IsNearlyZero())
			{
				UE_LOG(Rene, Log, TEXT("Character has arrived at sit target (2D Distance: %.2f). Starting blend."), DistanceToTarget2D);

				bIsMovingToSit = false;
				
				// [D] 보정 상태로 전환
				bIsBlendingToSitTarget = true;
				SitBlendElapsed = 0.0f;
				SitBlendStartLoc = GetActorLocation();
				SitBlendStartRot = GetActorRotation();

				// 보간 중 AI나 다른 이동 입력을 막기 위해 즉시 이동을 멈춤
				GetCharacterMovement()->StopMovementImmediately();
			}
		}
		// [D] 목표 지점으로 부드럽게 보정하는 로직
		else if (bIsBlendingToSitTarget)
		{
			SitBlendElapsed += DeltaTime;
			const float Alpha = FMath::Clamp(SitBlendElapsed / SitBlendDuration, 0.0f, 1.0f);

			// 위치와 회전을 부드럽게 보간
			const FVector NewLocation = FMath::Lerp(SitBlendStartLoc, TargetSitTransform.GetLocation(), Alpha);
			const FRotator NewRotation = FMath::RInterpTo(SitBlendStartRot, TargetSitTransform.GetRotation().Rotator(), DeltaTime, 10.0f); // RInterpTo로 부드러운 회전

			SetActorLocationAndRotation(NewLocation, NewRotation, false, nullptr, ETeleportType::None);

			// 보정이 완료되면
			if (Alpha >= 1.0f)
			{
				UE_LOG(Rene, Log, TEXT("Sit target blend finished. Setting final state."));
				bIsBlendingToSitTarget = false;

				// PlayerController의 자동 이동 플래그 해제
				if (ARene_PlayerController* PC = Cast<ARene_PlayerController>(Controller))
				{
					PC->SetAutoMoving(false);
				}

				// 서버에서 '앉음' 상태로 최종 변경
				Server_SetIsSitting(true);
			}
		}
	}

	// =================================================================
	//                 부드러운 회전 로직 (아래)
	// =================================================================
	// 회전 중일 때만 실행 (서버/클라이언트 모두에서 시각적으로 처리)
	if (bIsTurningToSit)
	{
		const FRotator CurrentRotation = GetActorRotation();
		const FRotator TargetRotation = TargetSitTransform.GetRotation().Rotator();

		// 목표 각도에 도달했는지 체크 (1도 이내 오차 허용)
		if (CurrentRotation.Equals(TargetRotation, 1.0f))
		{
			bIsTurningToSit = false; // 회전 완료
		}
		else
		{
			// 매끄러운 회전을 위해 RInterpTo 함수 사용
			const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, SitTurnInterpSpeed);
			SetActorRotation(NewRotation);
		}
	}
	
	// =================================================================
	//                 부드러운 카메라 이동 로직 (아래)
	// =================================================================
	if (bIsInterpolatingCamera)
	{
		if (FirstPersonCameraComponent)
		{
			const FVector CurrentLocation = FirstPersonCameraComponent->GetRelativeLocation();
			const FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetCameraLocation, DeltaTime, CameraInterpSpeed);
			FirstPersonCameraComponent->SetRelativeLocation(NewLocation);

			// 목표 위치에 도달했는지 체크 (0.1cm 이내 오차 허용)
			if (CurrentLocation.Equals(TargetCameraLocation, 0.1f))
			{
				bIsInterpolatingCamera = false; // 보간 완료
			}
		}
	}
}

void AUE_ReNeCharacter::StandUp()
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return;
	}

	// 서버에서 '앉음' 상태를 해제합니다.
	Server_SetIsSitting(false);
}

void AUE_ReNeCharacter::SetTargetSitTransform(const FTransform& NewTransform)
{
	// 서버에서만 실행되도록 보장
	if (HasAuthority())
	{
		TargetSitTransform = NewTransform;
		bIsMovingToSit = true;

		// =================================================================
		//                 걷기 속도 조절 로직 (아래)
		// =================================================================
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (MoveComp)
		{
			// 1. 현재 속도를 백업합니다.
			OriginalMaxWalkSpeed = MoveComp->MaxWalkSpeed;
			// 2. 걷는 속도(예: 150)로 변경합니다.
			MoveComp->MaxWalkSpeed = 180.0f;
		}
		// =================================================================
	}
}

void AUE_ReNeCharacter::Server_SetIsSitting_Implementation(bool bNewSittingState)
{
	bIsSitting = bNewSittingState;

	// 클라이언트에도 상태를 즉시 반영하기 위해 OnRep 함수를 서버에서도 호출
	OnRep_bIsSitting();
}

void AUE_ReNeCharacter::OnRep_bIsSitting()
{
	// 이 함수는 bIsSitting 값이 변경될 때 모든 클라이언트와 서버에서 호출됩니다.
	if (bIsSitting)
	{
		// 앉는 애니메이션 재생 또는 상태 변경 로직
		// LOGWARNF(TEXT("Player %s is now sitting."), *GetName());

		// =================================================================
		//                 통제권 전환 로직 (아래)
		// =================================================================

		// 1. 카메라 방향을 따라 캐릭터가 회전하는 기능을 끕니다.
		bUseControllerRotationYaw = false;
		// 2. 캐릭터 이동 컴포넌트가 멋대로 캐릭터를 회전시키지 않도록 합니다.
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// 3. 카메라가 컨트롤러의 회전을 따르지 않고, 자신이 붙어있는 폰(캐릭터)을 따르도록 합니다.
		if (FirstPersonCameraComponent)
		{
			FirstPersonCameraComponent->bUsePawnControlRotation = false;
		}
		// =================================================================

		// 캐릭터 이동을 비활성화하여 앉은 상태에서 움직이지 못하게 함
		GetCharacterMovement()->DisableMovement();

		// =================================================================
		//                 부드러운 회전 시작 (아래)
		// =================================================================
		// 즉시 회전하는 대신, 부드러운 회전을 시작하라는 플래그만 켭니다.
		bIsTurningToSit = true;
		// =================================================================

		// =================================================================
		//                 카메라 이동 시작 (아래)
		// =================================================================
		TargetCameraLocation = StandingCameraLocation + SittingCameraOffset;
		bIsInterpolatingCamera = true;
		// =================================================================
		
		// =================================================================
		//                 인터뷰 위젯 표시 요청 (아래)
		// =================================================================
		// 로컬 플레이어의 컨트롤러에서만 UI를 띄우도록 요청합니다.
		if (ARene_PlayerController* PC = Cast<ARene_PlayerController>(Controller))
		{
			if (PC->IsLocalController())
			{
				PC->ShowInterviewWidget();
			}
		}
		// =================================================================
	}
	else
	{
		// 일어나는 애니메이션 재생 또는 상태 변경 로직
		// LOGWARNF(TEXT("Player %s is now standing up."), *GetName());

		// =================================================================
		//                 통제권 전환 로직 (아래)
		// =================================================================

		// 1. 카메라 방향을 따라 캐릭터가 회전하는 기능을 다시 켭니다.
		bUseControllerRotationYaw = true;
		// 2. 캐릭터가 움직이는 방향으로 자연스럽게 회전하도록 되돌립니다.
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// 3. 카메라가 다시 컨트롤러의 회전을 따르도록 되돌립니다.
		if (FirstPersonCameraComponent)
		{
			FirstPersonCameraComponent->bUsePawnControlRotation = true;
		}
		// =================================================================

		// =================================================================
		//                 원래 속도로 복구 (아래)
		// =================================================================
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (MoveComp && OriginalMaxWalkSpeed > 0.0f)
		{
			MoveComp->MaxWalkSpeed = OriginalMaxWalkSpeed;
		}
		// =================================================================

		GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		// 일어설 때 회전 중이었다면 중지
		bIsTurningToSit = false;

		// =================================================================
		//                 카메라 이동 시작 (아래)
		// =================================================================
		TargetCameraLocation = StandingCameraLocation;
		bIsInterpolatingCamera = true;
		// =================================================================
	}
}
