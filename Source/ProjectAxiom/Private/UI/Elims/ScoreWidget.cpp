// Copyright Zel Suarez


#include "UI/Elims/ScoreWidget.h"
#include "Components/TextBlock.h"
#include "Player/AxiomPlayerController.h"

#include "Player/ShooterPlayerState.h"

void UScoreWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	AShooterPlayerState* PS = GetPlayerState();
	if (IsValid(PS))
	{
		PS->OnScoreChanged.AddDynamic(this, &ThisClass::OnScoreChanged);
	}
	else
	{
		AAxiomPlayerController* PC = Cast<AAxiomPlayerController>(GetOwningPlayer());
		if (IsValid(PC))
		{
			PC->OnPlayerStateReplicated.AddUniqueDynamic(this, &ThisClass::OnPlayerStateReplicated);
		}
	}
}

void UScoreWidget::OnPlayerStateReplicated()
{
	AShooterPlayerState* PS = GetPlayerState();
	if (IsValid(PS))
	{
		PS->OnScoreChanged.AddDynamic(this, &ThisClass::OnScoreChanged);
		OnScoreChanged(PS->GetScore());
	}
	
	AAxiomPlayerController* PC = Cast<AAxiomPlayerController>(GetOwningPlayer());
	if (IsValid(PC))
	{
		PC->OnPlayerStateReplicated.RemoveDynamic(this, &ThisClass::OnPlayerStateReplicated);
	}
}

AShooterPlayerState* UScoreWidget::GetPlayerState() const
{
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		return PC->GetPlayerState<AShooterPlayerState>();
	}
	
	return nullptr;
}

void UScoreWidget::OnScoreChanged(int32 Score)
{
	if (IsValid(Text_Score))
	{
		Text_Score->SetText(FText::AsNumber(Score));
	}
}
