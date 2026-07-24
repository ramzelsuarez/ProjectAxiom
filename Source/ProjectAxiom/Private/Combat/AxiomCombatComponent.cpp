// Copyright Zel Suarez


#include "Combat/AxiomCombatComponent.h"

#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"


UAxiomCombatComponent::UAxiomCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
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
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Cyan, 
		TEXT("Initiate_FireWeapon_Pressed"), 
		false);
}

void UAxiomCombatComponent::Initiate_FireWeapon_Released()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Cyan, 
		TEXT("Initiate_FireWeapon_Released"), 
		false);
}

void UAxiomCombatComponent::Initiate_Aim_Pressed()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Cyan, 
		TEXT("Initiate_Aim_Pressed"), 
		false);
}

void UAxiomCombatComponent::Initiate_Aim_Released()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Cyan, 
		TEXT("Initiate_Aim_Released"), 
		false);
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
		Inventory[0]->AttachToOwningPawn();
	}
}

void UAxiomCombatComponent::DestroyInventory()
{
	// TODO: Destroy the inventory once we made one.
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
