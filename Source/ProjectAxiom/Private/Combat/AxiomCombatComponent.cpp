// Copyright Zel Suarez


#include "Combat/AxiomCombatComponent.h"


UAxiomCombatComponent::UAxiomCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAxiomCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


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

