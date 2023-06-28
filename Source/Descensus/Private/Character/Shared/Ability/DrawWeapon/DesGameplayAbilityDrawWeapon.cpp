#include "Character/Shared/Ability/DrawWeapon/DesGameplayAbilityDrawWeapon.h"

#include "AbilitySystemComponent.h"
#include "DesGameplayTags.h"
#include "Character/Shared/Ability/Attack/DesGameplayAbilityAttack.h"
#include "Input/DesAbilityInputID.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Equip_Unarmed, "Equip.Unarmed")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_DrawWeapon, "Ability.DrawWeapon")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_DrawWeapon_Active, "Ability.DrawWeapon.Active")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_DrawWeapon_Active_Local, "Ability.DrawWeapon.Active.Local")

UDesGameplayAbilityDrawWeapon::UDesGameplayAbilityDrawWeapon()
{
	AbilityTags.AddTag(TAG_Ability_DrawWeapon);
	// ActivationOwnedTags.AddTag(TAG_Equip_Unarmed);
	AbilityInputID = EDesAbilityInputID::DrawWeapon;
	
	LocalActivationOwnedTags.AddTag(TAG_Ability_DrawWeapon_Active_Local);
	ReplicatedActivationOwnedTags.AddTag(TAG_Ability_DrawWeapon_Active);
}