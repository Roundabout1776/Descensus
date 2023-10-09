#include "Character/Ability/DesGameplayEffectStaminaRegen.h"

#include "DesConstants.h"
#include "Character/DesCharacterAttributeSet.h"

UDesGameplayEffectStaminaRegen::UDesGameplayEffectStaminaRegen()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = DEFAULT_EFFECT_PERIOD;

	Modifiers.Empty();
	auto StaminaModifier = FGameplayModifierInfo();
	StaminaModifier.Attribute = UDesCharacterAttributeSet::GetStaminaAttribute();
	StaminaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(1.0f * Period.Value);
	Modifiers.Add(StaminaModifier);
}
