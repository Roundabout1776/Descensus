#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"
#include "DesGameplayAbilityPlayerInscribe.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_PlayerInscribe)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_PlayerInscribe_Active)

UCLASS()
class DESCENSUS_API UDesGameplayAbilityPlayerInscribe : public UDesGameplayAbilityPrimaryBase
{
	GENERATED_BODY()

	FActiveGameplayEffectHandle EffectHandle;

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnTimeout(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo);

	UFUNCTION()
	void OnInputRelease(float TimeHeld);

public:
	UDesGameplayAbilityPlayerInscribe();
};
