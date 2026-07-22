// Copyright Zel Suarez

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTAXIOM_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	
	USkeletalMeshComponent* GetMesh1P() const;
	USkeletalMeshComponent* GetMesh3P() const;

protected:
	virtual void BeginPlay() override;
	
private:
	
	// Weapon Mesh: 1st Person view
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	// Weapon Mesh: 3rd Person view
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
};
