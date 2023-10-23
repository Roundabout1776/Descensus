#pragma once

#include "CoreMinimal.h"
#include "Character/Ability/DesGameplayAbilityHandsBase.h"
#include "DesGameplayAbility_Inscribe.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Equip_Inscribe)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inscribe)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inscribe_Active)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inscribe_Active_Local)

UCLASS()
class DESCENSUS_API UDesGameplayAbility_Inscribe : public UDesGameplayAbilityHandsBase
{
	GENERATED_BODY()

public:
	UDesGameplayAbility_Inscribe();
};
