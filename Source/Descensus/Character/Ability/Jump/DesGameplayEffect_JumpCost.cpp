#include "Character/Ability/Jump/DesGameplayEffect_JumpCost.h"

#include "Character/Ability/Jump/DesGameplayMMC_JumpCost.h"
#include "Character/DesCharacterAttributeSet.h"

UDesGameplayEffect_JumpCost::UDesGameplayEffect_JumpCost()
{
	Modifiers.Empty();

	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.Attribute = UDesCharacterAttributeSet::GetStaminaAttribute();
	FCustomCalculationBasedFloat ModifierMagnitude;
	ModifierMagnitude.CalculationClassMagnitude = UDesGameplayMMC_JumpCost::StaticClass();
	StaminaModifier.ModifierMagnitude = ModifierMagnitude;
	Modifiers.Add(StaminaModifier);
}
