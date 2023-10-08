#pragma once

#include "CoreMinimal.h"
#include "DesGameplayAbility.h"
#include "NativeGameplayTags.h"
#include "DesGameplayAbilityPrimaryBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Primary)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Primary_Active)

UCLASS()
class DESCENSUS_API UDesGameplayAbilityPrimaryBase : public UDesGameplayAbility
{
	GENERATED_BODY()

public:
	UDesGameplayAbilityPrimaryBase();
};
