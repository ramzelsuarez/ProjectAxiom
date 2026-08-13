// Copyright Zel Suarez


#include "Character/AxiomCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Combat/AxiomCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/WeaponData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"

AAxiomCharacter::AAxiomCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->MovementState.bCanCrouch = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bUsePawnControlRotation = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->SetupAttachment(SpringArm);
	FirstPersonCamera->bUsePawnControlRotation = false;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh1P");
	Mesh1P->SetupAttachment(FirstPersonCamera);
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;
	
	Combat = CreateDefaultSubobject<UAxiomCombatComponent>("Combat");
	Combat->SetIsReplicated(true);
	
	DefaultFieldOfView = 90.0f;
	TurningStatus = ETurningInPlace::NotTurning;
	bWeaponFirstReplicated = false;
}

void AAxiomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FirstPersonCamera->SetFieldOfView(DefaultFieldOfView);
	
	StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
}

void AAxiomCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (IsValid(Combat))
	{
		Combat->DestroyInventory();
	}
}

FRotator AAxiomCharacter::GetFixedAimRotation() const
{
	FRotator AimRotation = GetBaseAimRotation();
	if (AimRotation.Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0]
		const FVector2D InRange(270.f, 360.f);
		const FVector2D OutRange(-90.f, 0.f);
		AimRotation.Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AimRotation.Pitch);
	}
	
	return AimRotation;
}

bool AAxiomCharacter::HasCurrentWeapon() const
{
	return IsValid(Combat) && Combat->CurrentWeapon != nullptr;
}

void AAxiomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateTurnInPlaceParameters(DeltaTime);
	CalculateFABRIK_SocketTransform();
}

void AAxiomCharacter::CalculateTurnInPlaceParameters(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();
	
	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		FRotator CurrentAimRotation(0.f, GetBaseAimRotation().Yaw, 0.f);
		// StartingAimRotation initially set in BeginPlay
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		
		if (TurningStatus == ETurningInPlace::NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		
		TurnInPlace(DeltaTime); // interpolates the InterAO_Yaw value to 0
	}
	
	if (Speed > 0.f || bIsInAir)
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		
		FRotator AimRotation = GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
		TurningStatus = ETurningInPlace::NotTurning;
	}
	
	AO_Yaw *= -1.f;
}

void AAxiomCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningStatus = ETurningInPlace::Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningStatus = ETurningInPlace::Left;
	}
	if (TurningStatus != ETurningInPlace::NotTurning) // we are turning
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.0f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 5.f)
		{
			TurningStatus = ETurningInPlace::NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
	
}

void AAxiomCharacter::CalculateFABRIK_SocketTransform()
{
	if (IsValid(Combat) && IsValid(Combat->CurrentWeapon) && IsValid(Combat->CurrentWeapon->GetMesh3P()))
	{
		FABRIK_SocketTransform = Combat->CurrentWeapon->GetMesh3P()->GetSocketTransform("FABRIK_Socket", RTS_World);
		
		FVector OutLocation;
		FRotator OutRotation;
		GetMesh()->TransformToBoneSpace(
			"hand_r",
			FABRIK_SocketTransform.GetLocation(),
			FABRIK_SocketTransform.GetRotation().Rotator(),
			OutLocation, 
			OutRotation);
		FABRIK_SocketTransform.SetLocation(OutLocation);
		FABRIK_SocketTransform.SetRotation(OutRotation.Quaternion());
	}
}

void AAxiomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* AxiomInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	AxiomInputComponent->BindAction(CycleWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_CycleWeapon);
	AxiomInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_FireWeapon_Pressed);
	AxiomInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_FireWeapon_Released);
	AxiomInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_Aim_Pressed);
	AxiomInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_Aim_Released);
	AxiomInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_ReloadWeapon);
}

void AAxiomCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (IsValid(Combat))
	{
		Combat->SpawnInventory();
	}
}

void AAxiomCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (IsValid(Combat))
	{
		Combat->InitializeWeaponWidgets();
	}
}

FName AAxiomCharacter::GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const
{
	checkf(Combat->WeaponData, TEXT("No Weapon Data Asset - Please fill out BP_ShooterCharacter"));
	return Combat->WeaponData->GripPoints.FindChecked(WeaponType);
}

USkeletalMeshComponent* AAxiomCharacter::GetMesh1P_Implementation() const
{
	return Mesh1P;
}

USkeletalMeshComponent* AAxiomCharacter::GetMesh3P_Implementation() const
{
	return GetMesh();
}

void AAxiomCharacter::WeaponReplicated_Implementation()
{
	if (!bWeaponFirstReplicated)
	{
		bWeaponFirstReplicated = true;
		OnWeaponFirstReplicated.Broadcast(Combat->CurrentWeapon, Combat->bHitPlayer);
	}
}

AWeapon* AAxiomCharacter::GetCurrentWeapon_Implementation()
{
	return Combat->CurrentWeapon;
}

int32 AAxiomCharacter::GetReserveAmmo_Implementation() const
{
	return Combat->CurrentReserveAmmo;
}

void AAxiomCharacter::Notify_CycleWeapon_Implementation()
{
	Combat->Notify_CycleWeapon();
}

void AAxiomCharacter::Notify_ReloadWeapon_Implementation()
{
	Combat->Notify_ReloadWeapon();
}

void AAxiomCharacter::AddAmmo_Implementation(const FGameplayTag& WeaponType, int32 AmmoAmount)
{
	if (HasAuthority() && IsValid(Combat))
	{
		Combat->AddAmmo(WeaponType, AmmoAmount);
	}
}

void AAxiomCharacter::Input_CycleWeapon()
{
	Combat->Initiate_CycleWeapon();
}

void AAxiomCharacter::Input_ReloadWeapon()
{
	Combat->Initiate_ReloadWeapon();
}

void AAxiomCharacter::Input_FireWeapon_Pressed()
{
	Combat->Initiate_FireWeapon_Pressed();
}

void AAxiomCharacter::Input_FireWeapon_Released()
{
	Combat->Initiate_FireWeapon_Released();
}

void AAxiomCharacter::Input_Aim_Pressed()
{
	Combat->Initiate_Aim_Pressed();
	OnAim(true);
}

void AAxiomCharacter::Input_Aim_Released()
{
	Combat->Initiate_Aim_Released();
	OnAim(false);
}
