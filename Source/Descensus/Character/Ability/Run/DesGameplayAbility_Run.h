#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/DesGameplayAbility.h"
#include "DesGameplayAbility_Run.generated.h"

class UDesGameplayEffectRun;
class UAbilityTask_WaitInputRelease;
class UAbilityTask_NetworkSyncPoint;
class UAbilityTask_WaitDelay;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Run)

UCLASS()
class DESCENSUS_API UDesGameplayAbility_Run : public UDesGameplayAbility
{
	GENERATED_BODY()

	FActiveGameplayEffectHandle RunEffectHandle;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnInputRelease(float TimeHeld);

	UFUNCTION()
	void OnRun(bool bEligibleForRunning, float DeltaTime) const;

	UFUNCTION()
	void OnStaminaDepleted();

protected:
	UPROPERTY(EditDefaultsOnly)
	float StaminaCost;

public:
	UDesGameplayAbility_Run();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                FGameplayTagContainer* OptionalRelevantTags) const override;
};
