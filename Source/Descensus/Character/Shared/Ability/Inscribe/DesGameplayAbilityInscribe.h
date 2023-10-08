#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DesGameplayAbilityHandsBase.h"
#include "DesGameplayAbilityInscribe.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Equip_Inscribe)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inscribe)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inscribe_Active)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inscribe_Active_Local)

UCLASS()
class DESCENSUS_API UDesGameplayAbilityInscribe : public UDesGameplayAbilityHandsBase
{
	GENERATED_BODY()

public:
	UDesGameplayAbilityInscribe();
};
