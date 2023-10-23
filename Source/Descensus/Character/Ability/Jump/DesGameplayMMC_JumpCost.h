#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UObject/Object.h"
#include "DesGameplayMMC_JumpCost.generated.h"

UCLASS()
class DESCENSUS_API UDesGameplayMMC_JumpCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	FGameplayEffectAttributeCaptureDefinition MaxStaminaDef;

public:
	UDesGameplayMMC_JumpCost();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
