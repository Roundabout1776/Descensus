#include "Character/Shared/Ability/Run/DesGameplayAbilityRun.h"

#include "AbilitySystemComponent.h"
#include "DesLogging.h"
#include "DesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Shared/Ability/Run/DesAbilityTaskRun.h"
#include "Character/DesCharacter.h"
#include "Character/DesCharacterAttributeSet.h"
#include "Character/DesCharacterMovementComponent.h"
#include "Input/DesAbilityInputID.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Run, "Ability.Run")

UDesGameplayAbilityRun::UDesGameplayAbilityRun()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	AbilityTags.AddTag(TAG_Ability_Run);

	ReplicatedActivationOwnedTags.AddTag(TAG_Movement_Run);
	AbilityInputID = EDesAbilityInputID::Run;

	StaminaCost = 3.0f;
}

bool UDesGameplayAbilityRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags,
                                                FGameplayTagContainer* OptionalRelevantTags) const
{
	return GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(UDesCharacterAttributeSet::GetStaminaAttribute()) > StaminaCost &&
		Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDesGameplayAbilityRun::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	const auto Movement = Cast<UDesCharacterMovementComponent>(
		Cast<ADesCharacter>(ActorInfo->AvatarActor)->GetMovementComponent());
	Movement->SetRunning(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDesGameplayAbilityRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	const auto Movement = Cast<UDesCharacterMovementComponent>(
		Cast<ADesCharacter>(ActorInfo->AvatarActor)->GetMovementComponent());
	Movement->SetRunning(true);

	if (HasAuthority(&ActivationInfo))
	{
		const auto WaitStaminaChange = UAbilityTask_WaitAttributeChange::WaitForAttributeChangeWithComparison(
			this, UDesCharacterAttributeSet::GetStaminaAttribute(), FGameplayTag::EmptyTag, FGameplayTag::EmptyTag,
			EWaitAttributeChangeComparison::LessThanOrEqualTo, 0.0f, false);
		WaitStaminaChange->OnChange.AddDynamic(this, &ThisClass::OnStaminaDepleted);
		WaitStaminaChange->ReadyForActivation();

		const auto RunTask = UAbilityTask::NewAbilityTask<UDesAbilityTaskRun>(this);
		RunTask->OnRun.BindUObject(this, &ThisClass::OnRun);
		RunTask->ReadyForActivation();
	}

	const auto WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputRelease);
	WaitInputReleaseTask->ReadyForActivation();
}

void UDesGameplayAbilityRun::OnInputRelease(float TimeHeld)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UDesGameplayAbilityRun::OnStaminaDepleted()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UDesGameplayAbilityRun::OnRun(bool bEligibleForRunning, float DeltaTime) const
{
	if (bEligibleForRunning)
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyModToAttribute(UDesCharacterAttributeSet::GetStaminaAttribute(),
		                                                              EGameplayModOp::Additive,
		                                                              -StaminaCost * DeltaTime);
	}
}
