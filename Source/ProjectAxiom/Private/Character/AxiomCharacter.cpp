// Copyright Zel Suarez

#include "Character/AxiomCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Combat/AxiomCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/WeaponData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
}

void AAxiomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FirstPersonCamera->SetFieldOfView(DefaultFieldOfView);
	
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
	CalculateTurnInPlaceParameters();
	CalculateFABRIK_SocketTransform();
}

void AAxiomCharacter::CalculateTurnInPlaceParameters()
{
	// Get velocity, see if 0; 0 means standing still
	// See if we are falling
	
	// if standing still and not jumping
		// get current aim rotation
		// get delta aim rotation - the difference in rotation of my current aim rotation from the initial aim rotation
		// (initial aim rotation is calculated in BeginPlay)
		// Store the Yaw of the delta aim rotation (AO_Yaw)
		// if TurningStatus == NotTurning
			// Set InterpAO_Yaw to AO_Yaw
		// TurnInPlace() - interpolates the InterAO_Yaw value to 0
	
	// if running or jumping
		// reset initial aim rotation to the current actual aim rotation
		// AO_Yaw = 0
		// We also need a Movement Offset Yaw to feed out strafing blendspaces
		// Get Base Aim Rotation
		// Get our Movement Rotation - this is the rotation of our Velocity
		// Movement Offset Yaw = the delta between our movement rotation and our aim rotation
		// TurningStatus = NotTurning
	
}

// Turn In Place
	// if AO_Yaw > 90
		// TurningStatus = Right
	// else if AO_Yaw < -90
		// TurningStatus = Left
	// if TurningStatus != NotTurning (in other words, we are turning left or right)
		// Interpolate InterpAO_Yaw down to 0
		// AO_Yaw = InterpAO_Yaw
		// if Abs(AO_Yaw) < 5.f
			// TurningStatus = NotTurning
			// reset initial aim rotation to our actual aim rotation

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
