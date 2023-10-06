#include "Player/Ability/Drag/DesGameplayAbilityPlayerDrag.h"
#include "Player/Ability/Drag/DesAbilityTaskPlayerDrag.h"
#include "AbilitySystem/DesGameplayAbilityHandsBase.h"
#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "DesLogging.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_PlayerDrag, "Ability.PlayerDrag")

UDesGameplayAbilityPlayerDrag::UDesGameplayAbilityPlayerDrag() {
	AbilityTags.AddTag(TAG_Ability_PlayerDrag);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Hands);
    CancelAbilitiesWithTag.AddTag(TAG_Ability_Hands);
    CancelAbilitiesWithTag.AddTag(TAG_Ability_Primary);
}

void UDesGameplayAbilityPlayerDrag::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                                   bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    
    DES_LOG_BOOL("PlayerDrag: ", false)
}

void UDesGameplayAbilityPlayerDrag::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                        const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
    DES_LOG_BOOL("PlayerDrag: ", true)

	const auto WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputRelease);
	WaitInputReleaseTask->ReadyForActivation();

    const auto DragTask = UAbilityTask::NewAbilityTask<UDesAbilityTaskPlayerDrag>(this);
    // DragTask->OnRun.BindUObject(this, &ThisClass::OnRun);
    DragTask->ReadyForActivation();
}

void UDesGameplayAbilityPlayerDrag::OnInputRelease(float TimeHeld)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}