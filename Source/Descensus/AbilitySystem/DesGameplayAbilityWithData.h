#pragma once

#include "CoreMinimal.h"
#include "DesGameplayAbility.h"
#include "DesGameplayAbilityWithData.generated.h"

UCLASS()
class DESCENSUS_API UDesGameplayAbilityWithData : public UDesGameplayAbility
{
	GENERATED_BODY()

	FDelegateHandle NotifyTargetDataReadyDelegateHandle;

protected:
	virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                               bool bWasCancelled) override;
	virtual void ActivateAbilityServer(const FGameplayAbilitySpecHandle Handle,
	                                   const FGameplayAbilityActorInfo* ActorInfo,
	                                   const FGameplayAbilityActivationInfo ActivationInfo,
	                                   const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& LocalTargetDataHandle,
	                                           FGameplayTag GameplayTag);
	virtual void NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
};
