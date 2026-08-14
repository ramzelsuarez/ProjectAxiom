#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AxiomGameModeBase.generated.h"


UCLASS()
class PROJECTAXIOM_API AAxiomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	void RequestRespawn(ACharacter* Character, AController* Controller);
};