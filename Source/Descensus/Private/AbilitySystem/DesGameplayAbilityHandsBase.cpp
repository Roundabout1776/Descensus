#include "AbilitySystem/DesGameplayAbilityHandsBase.h"

#include "AbilitySystemComponent.h"
#include "DesLogging.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Hands, "Ability.Hands")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Hands_Active, "Ability.Hands.Active")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Hands_Active_Local, "Ability.Hands.Active.Local")

UDesGameplayAbilityHandsBase::UDesGameplayAbilityHandsBase()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	AbilityTags.AddTag(TAG_Ability_Hands);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Hands);
	ActivationBlockedTags.AddTag(TAG_Ability_Primary_Active);

	LocalActivationOwnedTags.AddTag(TAG_Ability_Hands_Active_Local);
	ReplicatedActivationOwnedTags.AddTag(TAG_Ability_Hands_Active);
}

void UDesGameplayAbilityHandsBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	DES_LOG_INT("ActivateAbility:IsLocallyControlled", IsLocallyControlled())

	State = EHandsAbilityState::TransitionIn;

	const auto ReadyTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ReadyTaskMontage"), ReadyMontage);
	ReadyTask->OnCompleted.AddDynamic(this, &ThisClass::OnReady);
	ReadyTask->ReadyForActivation();
	
	// const auto PrepareTask = UAbilityTask_WaitDelay::WaitDelay(this, TransitionInTime);
	// PrepareTask->OnFinish.AddDynamic(this, &ThisClass::OnReady);
	// PrepareTask->ReadyForActivation();
}

void UDesGameplayAbilityHandsBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	DES_LOG_INT("EndAbility:IsLocallyControlled", IsLocallyControlled())
	State = EHandsAbilityState::NotReady;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UDesGameplayAbilityHandsBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayTagContainer* SourceTags,
                                                      const FGameplayTagContainer* TargetTags,
                                                      FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDesGameplayAbilityHandsBase::OnInputPress(float TimeWaited)
{
	DES_LOG_INT("OnInputPress:IsLocallyControlled", IsLocallyControlled())
	EndAbilityCurrent();
}

void UDesGameplayAbilityHandsBase::OnReady()
{
	/** @TODO allow primary activation now */
	State = EHandsAbilityState::Ready;

	if (IsLocallyControlled())
	{
		const auto InputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
		InputPressTask->OnPress.AddDynamic(this, &ThisClass::OnInputPress);
		InputPressTask->ReadyForActivation();
	}
}

void UDesGameplayAbilityHandsBase::ActivateWhileReady(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbilityCurrent();
	// CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}
