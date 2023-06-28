#include "AbilitySystem/DesGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "GameplayTask.h"

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

void UDesGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
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
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

UDesGameplayAbility::UDesGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// bServerRespectsRemoteAbilityCancellation = false;
}
