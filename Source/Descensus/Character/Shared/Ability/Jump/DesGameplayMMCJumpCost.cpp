#include "Character/Shared/Ability/Jump/DesGameplayMMCJumpCost.h"

#include "Character/DesCharacterAttributeSet.h"

UDesGameplayMMCJumpCost::UDesGameplayMMCJumpCost()
{
	MaxStaminaDef.AttributeToCapture = UDesCharacterAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxStaminaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxStaminaDef);
}

float UDesGameplayMMCJumpCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FAggregatorEvaluateParameters EvaluateParameters;
	auto MaxStamina = 0.0f;
	GetCapturedAttributeMagnitude(MaxStaminaDef, Spec, EvaluateParameters, MaxStamina);

	return MaxStamina * -0.1f;
}
