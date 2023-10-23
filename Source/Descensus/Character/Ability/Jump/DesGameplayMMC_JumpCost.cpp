#include "Character/Ability/Jump/DesGameplayMMC_JumpCost.h"

#include "Character/DesCharacterAttributeSet.h"

UDesGameplayMMC_JumpCost::UDesGameplayMMC_JumpCost()
{
	MaxStaminaDef.AttributeToCapture = UDesCharacterAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxStaminaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxStaminaDef);
}

float UDesGameplayMMC_JumpCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FAggregatorEvaluateParameters EvaluateParameters;
	auto MaxStamina = 0.0f;
	GetCapturedAttributeMagnitude(MaxStaminaDef, Spec, EvaluateParameters, MaxStamina);

	return MaxStamina * -0.1f;
}
