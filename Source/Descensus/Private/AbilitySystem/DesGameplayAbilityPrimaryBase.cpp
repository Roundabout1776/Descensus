#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Primary, "Ability.Primary")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Primary_Active, "Ability.Primary.Active")

UDesGameplayAbilityPrimaryBase::UDesGameplayAbilityPrimaryBase()
{
	AbilityTags.AddTag(TAG_Ability_Primary);
	ActivationBlockedTags.AddTag(TAG_Ability_Primary);
	ReplicatedActivationOwnedTags.AddTag(TAG_Ability_Primary_Active);
}
