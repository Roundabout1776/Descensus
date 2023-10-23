#include "Character/Ability/Run/DesGameplayAbility_Run.h"

#include "AbilitySystemComponent.h"
#include "DesLogging.h"
#include "DesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Ability/Run/DesAbilityTask_Run.h"
#include "Character/DesCharacter.h"
#include "Character/DesCharacterAttributeSet.h"
#include "Character/DesCharacterMovementComponent.h"
#include "Input/DesAbilityInputID.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Run, "Ability.Run")

UDesGameplayAbility_Run::UDesGameplayAbility_Run()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	AbilityTags.AddTag(TAG_Ability_Run);

	ReplicatedActivationOwnedTags.AddTag(TAG_Movement_Run);
	AbilityInputID = EDesAbilityInputID::Run;

	StaminaCost = 3.0f;
}

bool UDesGameplayAbility_Run::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags,
                                                FGameplayTagContainer* OptionalRelevantTags) const
{
	return GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(UDesCharacterAttributeSet::GetStaminaAttribute()) > StaminaCost &&
		Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDesGameplayAbility_Run::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	const auto Movement = Cast<UDesCharacterMovementComponent>(
		Cast<ADesCharacter>(ActorInfo->AvatarActor)->GetMovementComponent());
	Movement->SetRunning(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDesGameplayAbility_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

		const auto RunTask = UAbilityTask::NewAbilityTask<UDesAbilityTask_Run>(this);
		RunTask->OnRun.BindUObject(this, &ThisClass::OnRun);
		RunTask->ReadyForActivation();
	}

	const auto WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputRelease);
	WaitInputReleaseTask->ReadyForActivation();
}

void UDesGameplayAbility_Run::OnInputRelease(float TimeHeld)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UDesGameplayAbility_Run::OnStaminaDepleted()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UDesGameplayAbility_Run::OnRun(bool bEligibleForRunning, float DeltaTime) const
{
	if (bEligibleForRunning)
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyModToAttribute(UDesCharacterAttributeSet::GetStaminaAttribute(),
		                                                              EGameplayModOp::Additive,
		                                                              -StaminaCost * DeltaTime);
	}
}
