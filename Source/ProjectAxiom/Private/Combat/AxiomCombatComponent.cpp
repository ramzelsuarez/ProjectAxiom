// Copyright Zel Suarez


#include "Combat/AxiomCombatComponent.h"

#include "Data/WeaponData.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"


UAxiomCombatComponent::UAxiomCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceLength = 20'000;
	
}

void UAxiomCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UAxiomCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UAxiomCombatComponent, Inventory);
	DOREPLIFETIME(UAxiomCombatComponent, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UAxiomCombatComponent, bAiming, COND_SkipOwner);
}

void UAxiomCombatComponent::Initiate_CycleWeapon()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Cyan, 
		TEXT("Initiate_CycleWeapon"), 
		false);
}

void UAxiomCombatComponent::Initiate_ReloadWeapon()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Cyan, 
		TEXT("Initiate_ReloadWeapon"), 
		false);
}

void UAxiomCombatComponent::Initiate_FireWeapon_Pressed()
{
	Local_FireWeapon();
}

void UAxiomCombatComponent::Local_FireWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	ensure (IsValid(WeaponData));
	
	UAnimMontage* Montage1P = WeaponData->FirstPersonMontages.FindChecked(CurrentWeapon->WeaponType).FireMontage;
	USkeletalMeshComponent* Mesh1P = IPlayerInterface::Execute_GetMesh1P(GetOwner());
	if (IsValid(Montage1P) && IsValid(Mesh1P))
	{
		Mesh1P->GetAnimInstance()->Montage_Play(Montage1P);
	}
	
	FHitResult Hit;
	CurrentWeapon->WeaponTrace(Hit, TraceLength);
	
	EPhysicalSurface ImpactSurfaceType = Hit.PhysMaterial.IsValid(false) ? Hit.PhysMaterial->SurfaceType.GetValue():SurfaceType1;
	CurrentWeapon->Local_Fire(Hit.ImpactPoint, Hit.ImpactNormal, ImpactSurfaceType, true);
	
	Server_FireWeapon(Hit);
}

void UAxiomCombatComponent::Server_FireWeapon_Implementation(const FHitResult& Hit)
{
	Multicast_FireWeapon(Hit);
}

void UAxiomCombatComponent::Multicast_FireWeapon_Implementation(const FHitResult& Hit)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn->IsLocallyControlled())
	{
		// do locally-controlled stuff
	}
	else
	{
		ensure (IsValid(WeaponData));
		
		EPhysicalSurface ImpactSurfaceType = Hit.PhysMaterial.IsValid(false) ? Hit.PhysMaterial->SurfaceType.GetValue():SurfaceType1;
		CurrentWeapon->Local_Fire(Hit.ImpactPoint, Hit.ImpactNormal, ImpactSurfaceType, false);
	
		UAnimMontage* Montage3P = WeaponData->ThirdPersonMontages.FindChecked(CurrentWeapon->WeaponType).FireMontage;
		USkeletalMeshComponent* Mesh3P = IPlayerInterface::Execute_GetMesh3P(GetOwner());
		if (IsValid(Montage3P) && IsValid(Mesh3P))
		{
			Mesh3P->GetAnimInstance()->Montage_Play(Montage3P);
		}
	}
}

void UAxiomCombatComponent::Initiate_FireWeapon_Released()
{
	
}

void UAxiomCombatComponent::Initiate_Aim_Pressed()
{
	Local_Aim(true);
	Server_Aim(true);
}

void UAxiomCombatComponent::Initiate_Aim_Released()
{
	Local_Aim(false);
	Server_Aim(false);
}

void UAxiomCombatComponent::Server_Aim_Implementation(bool bPressed)
{
	Local_Aim(bPressed);
}

void UAxiomCombatComponent::Local_Aim(bool bPressed)
{
	bAiming = bPressed;
}

void UAxiomCombatComponent::Equip(AWeapon* Weapon)
{
	CurrentWeapon = Weapon;
	CurrentWeapon->AttachToOwningPawn();
}

void UAxiomCombatComponent::SpawnInventory()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) return;
	
	for (TSubclassOf<AWeapon>& WeaponClass : DefaultWeaponClasses)
	{
		AWeapon* Weapon = SpawnWeapon(WeaponClass);
		Inventory.AddUnique(Weapon);
	}
	
	if (Inventory.Num() > 0)
	{
		Equip(Inventory[0]);
	}
}

void UAxiomCombatComponent::DestroyInventory()
{
	for (AWeapon* Weapon : Inventory)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}
}

void UAxiomCombatComponent::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
	if (!IsValid(CurrentWeapon)) return;
	CurrentWeapon->AttachToOwningPawn();
}

AWeapon* UAxiomCombatComponent::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const
{
	AActor* OwningActor = GetOwner();
	if (!IsValid(OwningActor)) return nullptr;
	if (OwningActor->GetLocalRole() < ROLE_Authority) return nullptr;
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(OwningActor);
	SpawnInfo.Owner = OwningActor;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnInfo);
}
