#include "Character/Ability/Inscribe/DesGameplayAbility_Inscribe.h"

#include "Input/DesAbilityInputID.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Equip_Inscribe, "Equip.Inscribe")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Inscribe, "Ability.Inscribe")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Inscribe_Active, "Ability.Inscribe.Active")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Inscribe_Active_Local, "Ability.Inscribe.Active.Local")

UDesGameplayAbility_Inscribe::UDesGameplayAbility_Inscribe()
{
	AbilityTags.AddTag(TAG_Ability_Inscribe);
	ActivationOwnedTags.AddTag(TAG_Equip_Inscribe);
	AbilityInputID = EDesAbilityInputID::Inscribe;

	LocalActivationOwnedTags.AddTag(TAG_Ability_Inscribe_Active_Local);
	ReplicatedActivationOwnedTags.AddTag(TAG_Ability_Inscribe_Active);
}
