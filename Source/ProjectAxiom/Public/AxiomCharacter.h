// Copyright Zel Suarez

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AxiomCharacter.generated.h"

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
	
};
