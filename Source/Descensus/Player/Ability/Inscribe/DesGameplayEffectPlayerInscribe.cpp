#include "Player/Ability/Inscribe/DesGameplayEffectPlayerInscribe.h"

UDesGameplayEffectPlayerInscribe::UDesGameplayEffectPlayerInscribe()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(10.0f);
}
