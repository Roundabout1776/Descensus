#include "Character/Ability/Attack/DesGameplayAbilityAttack.h"

#include "DesGameplayTags.h"
#include "DesLogging.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Ability/DrawWeapon/DesGameplayAbilityDrawWeapon.h"
#include "Item/DesWeaponType.h"
#include "Player/DesPlayerCharacter.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack, "Ability.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Character_State_Attacking, "Character.State.Attacking")

UDesGameplayAbilityAttack::UDesGameplayAbilityAttack()
{
	AbilityTags.AddTag(TAG_Ability_Attack);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Hands);
	ActivationRequiredTags.AddTag(TAG_Equip_Unarmed);
	ReplicatedActivationOwnedTags.AddTag(TAG_Character_State_Attacking);
}

void UDesGameplayAbilityAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

void UDesGameplayAbilityAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDesGameplayAbilityAttack::OnMontageCompleted()
{
	EndAbilityCurrent();
}

void UDesGameplayAbilityAttack::OnMontageCancelled()
{
	EndAbilityCurrent();
}

void UDesGameplayAbilityAttack::OnMontageInterrupted()
{
	EndAbilityCurrent();
}
