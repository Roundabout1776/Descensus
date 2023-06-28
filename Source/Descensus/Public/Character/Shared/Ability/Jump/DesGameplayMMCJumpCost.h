#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UObject/Object.h"
#include "DesGameplayMMCJumpCost.generated.h"

UCLASS()
class DESCENSUS_API UDesGameplayMMCJumpCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	FGameplayEffectAttributeCaptureDefinition MaxStaminaDef;

public:
	UDesGameplayMMCJumpCost();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
