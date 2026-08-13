// Copyright Zel Suarez


#include "UI/ReserveAmmo.h"

#include "Character/AxiomCharacter.h"
#include "Combat/AxiomCombatComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInterface.h"
#include "Weapon/Weapon.h"

void UReserveAmmo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Image_WeaponIcon->SetRenderOpacity(0.f);
	Text_Ammo->SetRenderOpacity(0.f);
	
	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
	
	AAxiomCharacter* AxiomCharacter = Cast<AAxiomCharacter>(GetOwningPlayer()->GetPawn());
	if (!IsValid(AxiomCharacter)) return;
	OnPossessedPawnChanged(nullptr, AxiomCharacter);
	
	if (AxiomCharacter->HasWeaponFirstReplicated())
	{
		AWeapon* Weapon = IPlayerInterface::Execute_GetCurrentWeapon(AxiomCharacter);
		if (IsValid(Weapon))
		{
			OnCurrentReserveAmmoChanged(IPlayerInterface::Execute_GetReserveAmmo(AxiomCharacter), Weapon->Ammo, Weapon->WeaponIcon);
		}
	}
	else
	{
		AxiomCharacter->OnWeaponFirstReplicated.AddDynamic(this, &ThisClass::OnWeaponFirstReplicated);
	}
	if (AxiomCharacter->HasAuthority())
	{
		AWeapon* Weapon = IPlayerInterface::Execute_GetCurrentWeapon(AxiomCharacter);
		if (!IsValid(Weapon)) return;
		OnCurrentReserveAmmoChanged(IPlayerInterface::Execute_GetReserveAmmo(AxiomCharacter), Weapon->Ammo, Weapon->WeaponIcon);
	}
}

void UReserveAmmo::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	UAxiomCombatComponent* OldPawnCombat = UAxiomCombatComponent::FindCombatComponent(OldPawn);
	if (IsValid(OldPawnCombat))
	{
		OldPawnCombat->OnCurrentReserveAmmoChanged.RemoveDynamic(this, &ThisClass::OnCurrentReserveAmmoChanged);
		OldPawnCombat->OnRoundFired.RemoveDynamic(this, &ThisClass::OnRoundFired);
	}
	UAxiomCombatComponent* NewPawnCombat = UAxiomCombatComponent::FindCombatComponent(NewPawn);
	if (IsValid(NewPawnCombat))
	{
		Image_WeaponIcon->SetRenderOpacity(1.f);
		Text_Ammo->SetRenderOpacity(1.f);
		NewPawnCombat->OnCurrentReserveAmmoChanged.AddDynamic(this, &ThisClass::OnCurrentReserveAmmoChanged);
		NewPawnCombat->OnRoundFired.AddDynamic(this, &ThisClass::OnRoundFired);
	}
}

void UReserveAmmo::OnCurrentReserveAmmoChanged(int32 RoundsInReserve, int32 RoundsInWeapon, UMaterialInterface* WeaponIconMaterial)
{
	if (IsValid(WeaponIconMaterial))
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(WeaponIconMaterial);
		if (IsValid(Image_WeaponIcon))
		{
			Image_WeaponIcon->SetBrush(Brush);
		}
	}
	
	if (IsValid(Text_Ammo))
	{
		FText AmmoText = FText::Format(NSLOCTEXT("AmmoText", "AmmoKey", "{0}/{1}"), RoundsInWeapon, RoundsInReserve);
		Text_Ammo->SetText(AmmoText);
	}
}

void UReserveAmmo::OnRoundFired(int32 RoundsCurrent, int32 RoundsMax, int32 RoundsInReserve)
{
	if (IsValid(Text_Ammo))
	{
		FText AmmoText = FText::Format(NSLOCTEXT("AmmoText", "AmmoKey", "{0}/{1}"), RoundsCurrent, RoundsInReserve);
		Text_Ammo->SetText(AmmoText);
	}
}

void UReserveAmmo::OnWeaponFirstReplicated(AWeapon* Weapon, bool bTargetingPlayer)
{
	AAxiomCharacter* AxiomCharacter = Cast<AAxiomCharacter>(GetOwningPlayer()->GetPawn());
	if (!IsValid(AxiomCharacter)) return;
	
	OnCurrentReserveAmmoChanged(IPlayerInterface::Execute_GetReserveAmmo(AxiomCharacter), Weapon->Ammo, Weapon->WeaponIcon);
}
