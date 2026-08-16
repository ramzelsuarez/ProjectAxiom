// Copyright Zel Suarez

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Player/ShooterPlayerState.h"
#include "ShooterGameStateBase.generated.h"


UCLASS()
class PROJECTAXIOM_API AShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AShooterGameStateBase();
	
	bool HasFirstBloodBeenHad() const;
	void UpdateLeader();
	AShooterPlayerState* GetSoleLeader() const;
	bool IsTiedForTheLead(AShooterPlayerState* PlayerState);
private:
	
	bool bHasFirstBloodBeenHad;
	
	UPROPERTY()
	TArray<TObjectPtr<AShooterPlayerState>> Leaders;
};
