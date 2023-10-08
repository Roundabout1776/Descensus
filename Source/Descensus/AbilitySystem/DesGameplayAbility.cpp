#include "AbilitySystem/DesGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GameplayTask.h"

UDesGameplayAbility::UDesGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDesGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                      const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* Comp = ActorInfo->AbilitySystemComponent.Get();

	Comp->AddLooseGameplayTags(NonReplicatedActivationOwnedTags);
	Comp->AddReplicatedLooseGameplayTags(ReplicatedActivationOwnedTags);
	Comp->AddLooseGameplayTags(ReplicatedActivationOwnedTags);
	if (IsLocallyControlled())
	{
		Comp->AddLooseGameplayTags(LocalActivationOwnedTags);
	}

	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UDesGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData && bHasBlueprintActivateFromEvent)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbilityFromEvent(*TriggerEventData);
		return;
	}
	if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
		return;
	}

	if (ActorInfo->IsLocallyControlledPlayer())
	{
		ActivateAbilityLocalPlayer(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else if (ActorInfo->IsNetAuthority())
	{
		ActivateAbilityServer(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}
}

void UDesGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount == 0)
		{
			if (UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
			{
				AbilitySystemComponent->RemoveLooseGameplayTags(NonReplicatedActivationOwnedTags);
				AbilitySystemComponent->RemoveReplicatedLooseGameplayTags(ReplicatedActivationOwnedTags);
				AbilitySystemComponent->RemoveLooseGameplayTags(ReplicatedActivationOwnedTags);
				if (IsLocallyControlled())
				{
					AbilitySystemComponent->RemoveLooseGameplayTags(LocalActivationOwnedTags);
				}
			}
			EndAbilityCleanup(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UDesGameplayAbility::EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UDesGameplayAbility::ActivateAbilityServer(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
}

void UDesGameplayAbility::ActivateAbilityLocalPlayer(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
}

void UDesGameplayAbility::EndAllTasks()
{
	for (int32 TaskIdx = ActiveTasks.Num() - 1; TaskIdx >= 0 && ActiveTasks.Num() > 0; --TaskIdx)
	{
		if (UGameplayTask* Task = ActiveTasks[TaskIdx])
		{
			Task->TaskOwnerEnded();
		}
	}
	ActiveTasks.Reset();
}

void UDesGameplayAbility::EndAbilityCurrent(const bool bReplicateEndAbility, const bool bWasCancelled)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
	           bReplicateEndAbility, bWasCancelled);
}
