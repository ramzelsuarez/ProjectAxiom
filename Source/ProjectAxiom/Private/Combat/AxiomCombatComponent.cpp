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
}

void UAxiomCombatComponent::Initiate_FireWeapon_Pressed()
{
}

void UAxiomCombatComponent::Initiate_FireWeapon_Released()
{
}

void UAxiomCombatComponent::Initiate_Aim_Pressed()
{
}

void UAxiomCombatComponent::Initiate_Aim_Released()
{
}

