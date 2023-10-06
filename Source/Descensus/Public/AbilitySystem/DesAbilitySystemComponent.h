#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DesAbilitySystemComponent.generated.h"

UCLASS()
class DESCENSUS_API UDesAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void PressAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass);
	virtual void ReleaseAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass);
	virtual void PressAbilitySpec(FGameplayAbilitySpec& Spec);
	virtual void ReleaseAbilitySpec(FGameplayAbilitySpec& Spec);
	virtual void PressAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bPressOnlyActive);
	virtual void ReleaseAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bReleaseOnlyActive);
};
