#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Character/Ability/DesGameplayAbilityHandsBase.h"
#include "DesGameplayAbility_DrawWeapon.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Equip_Unarmed)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_DrawWeapon)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_DrawWeapon_Active)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_DrawWeapon_Active_Local)

UCLASS()
class DESCENSUS_API UDesGameplayAbility_DrawWeapon : public UDesGameplayAbilityHandsBase
{
	GENERATED_BODY()

public:
	UDesGameplayAbility_DrawWeapon();
};
