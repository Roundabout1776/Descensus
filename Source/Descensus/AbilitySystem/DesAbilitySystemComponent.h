#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DesAbilitySystemComponent.generated.h"

UCLASS()
class DESCENSUS_API UDesAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool bDefaultAbilitiesGiven = false;
	bool bDefaultEffectsApplied = false;

	virtual void PressAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass);
	virtual void ReleaseAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass);
	virtual void PressAbilitySpec(FGameplayAbilitySpec& Spec);
	virtual void ReleaseAbilitySpec(FGameplayAbilitySpec& Spec);
	virtual void PressAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bPressOnlyActive);
	virtual void ReleaseAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bReleaseOnlyActive);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag,
	                             const FGameplayCueParameters& GameplayCueParameters) const;

	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void AddGameplayCueLocal(const FGameplayTag GameplayCueTag,
	                         const FGameplayCueParameters& GameplayCueParameters) const;

	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag,
	                            const FGameplayCueParameters& GameplayCueParameters) const;
};
