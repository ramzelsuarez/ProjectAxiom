// Copyright Zel Suarez

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AxiomCharacter.generated.h"

class UAxiomCombatComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTAXIOM_API AAxiomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAxiomCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAxiomCombatComponent> Combat;
	
	// 1st person view (arms)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;
};
