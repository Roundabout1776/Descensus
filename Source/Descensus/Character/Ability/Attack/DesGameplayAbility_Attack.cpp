#include "Character/Ability/Attack/DesGameplayAbility_Attack.h"

#include "DesGameplayTags.h"
#include "DesLogging.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Ability/DrawWeapon/DesGameplayAbility_DrawWeapon.h"
#include "Items/DesWeaponType.h"
#include "Player/DesPlayerCharacter.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack, "Ability.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Character_State_Attacking, "Character.State.Attacking")

UDesGameplayAbility_Attack::UDesGameplayAbility_Attack()
{
	AbilityTags.AddTag(TAG_Ability_Attack);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Hands);
	ActivationRequiredTags.AddTag(TAG_Equip_Unarmed);
	ReplicatedActivationOwnedTags.AddTag(TAG_Character_State_Attacking);
}

void UDesGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	if (const auto PlayerCharacter = Cast<ADesPlayerCharacter>(ActorInfo->AvatarActor))
	{
		const auto AnimMontage = Montages.Find(EDesWeaponType::Unarmed);
		if (!AnimMontage)
		{
			EndAbilityCurrent();
			return;
		}

		const auto PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, TEXT("AttackMontage"), *AnimMontage);
		PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
		PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
		PlayMontageTask->ReadyForActivation();
	}
}

void UDesGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDesGameplayAbility_Attack::OnMontageCompleted()
{
	EndAbilityCurrent();
}

void UDesGameplayAbility_Attack::OnMontageCancelled()
{
	EndAbilityCurrent();
}

void UDesGameplayAbility_Attack::OnMontageInterrupted()
{
	EndAbilityCurrent();
}
