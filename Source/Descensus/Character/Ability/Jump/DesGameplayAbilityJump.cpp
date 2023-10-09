#include "Character/Ability/Jump/DesGameplayAbilityJump.h"

#include "DesGameplayTags.h"
#include "Character/Ability/Jump/DesGameplayEffectJumpCost.h"
#include "Character/DesCharacter.h"
#include "Input/DesAbilityInputID.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Jump, "Ability.Jump")

UDesGameplayAbilityJump::UDesGameplayAbilityJump()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	AbilityTags.AddTag(TAG_Ability_Jump);
	CostGameplayEffectClass = UDesGameplayEffectJumpCost::StaticClass();
	AbilityInputID = EDesAbilityInputID::Jump;
}

void UDesGameplayAbilityJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	const auto Character = Cast<ADesCharacter>(ActorInfo->AvatarActor);

	if (CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		Character->Jump();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UDesGameplayAbilityJump::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UDesGameplayAbilityJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayTagContainer* SourceTags,
                                                 const FGameplayTagContainer* TargetTags,
                                                 FGameplayTagContainer* OptionalRelevantTags) const
{
	return Cast<ADesCharacter>(ActorInfo->AvatarActor)->CanJump() && Super::CanActivateAbility(
		Handle, ActorInfo, SourceTags, TargetTags,
		OptionalRelevantTags);
}
