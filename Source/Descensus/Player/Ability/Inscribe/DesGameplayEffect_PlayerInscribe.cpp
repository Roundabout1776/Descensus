#include "Player/Ability/Inscribe/DesGameplayEffect_PlayerInscribe.h"

UDesGameplayEffect_PlayerInscribe::UDesGameplayEffect_PlayerInscribe()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(10.0f);
}
