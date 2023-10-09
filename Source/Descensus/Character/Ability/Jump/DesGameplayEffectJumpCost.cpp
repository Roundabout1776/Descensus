#include "Character/Ability/Jump/DesGameplayEffectJumpCost.h"

#include "Character/Ability/Jump/DesGameplayMMCJumpCost.h"
#include "Character/DesCharacterAttributeSet.h"

UDesGameplayEffectJumpCost::UDesGameplayEffectJumpCost()
{
	Modifiers.Empty();

	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.Attribute = UDesCharacterAttributeSet::GetStaminaAttribute();
	FCustomCalculationBasedFloat ModifierMagnitude;
	ModifierMagnitude.CalculationClassMagnitude = UDesGameplayMMCJumpCost::StaticClass();
	StaminaModifier.ModifierMagnitude = ModifierMagnitude;
	Modifiers.Add(StaminaModifier);
}
