#include "Player/Ability/Inscribe/DesGameplayAbilityPlayerInscribe.h"

#include "AbilitySystemComponent.h"
#include "DesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Ability/Inscribe/DesGameplayAbilityInscribe.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/Ability/Inscribe/DesGameplayEffectPlayerInscribe.h"
#include "Player/DesPlayerController.h"
#include "UI/DesHUD.h"
#include "Player/Ability/Inscribe/DesAbilityTaskPlayerInscribe.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_PlayerInscribe, "Ability.PlayerInscribe")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_PlayerInscribe_Active, "Ability.PlayerInscribe.Active")

UDesGameplayAbilityPlayerInscribe::UDesGameplayAbilityPlayerInscribe()
{
	AbilityTags.AddTag(TAG_Ability_PlayerInscribe);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Hands);
	ActivationRequiredTags.AddTag(TAG_Ability_Inscribe_Active);
	ReplicatedActivationOwnedTags.AddTag(TAG_Ability_PlayerInscribe_Active);
}

void UDesGameplayAbilityPlayerInscribe::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                                   bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(EffectHandle);
}

void UDesGameplayAbilityPlayerInscribe::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                        const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	EffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo,
	                                          GetDefault<UDesGameplayEffectPlayerInscribe>(), 0.0f);

	if (HasAuthority(&ActivationInfo))
	{
		const auto WaitTimeout = UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(
			this, EffectHandle);
		WaitTimeout->OnRemoved.AddDynamic(this, &ThisClass::OnTimeout);
		WaitTimeout->ReadyForActivation();
	}

	if (ActorInfo->IsLocallyControlledPlayer())
	{
		const auto PlayerCharacter = Cast<ADesPlayerCharacter>(ActorInfo->AvatarActor.Get());
		const auto PlayerController = Cast<ADesPlayerController>(ActorInfo->PlayerController);
		const auto PlayerInscribe = UDesAbilityTaskPlayerInscribe::PlayerInscribe(
			this, PlayerCharacter->GetInscriptionComponent(),
			PlayerController->GetHUD<ADesHUD>()->GetInscriptionCanvas(), PlayerController);
		PlayerInscribe->ReadyForActivation();

		const auto WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		WaitInputRelease->OnRelease.AddDynamic(this, &ThisClass::OnInputRelease);
		WaitInputRelease->ReadyForActivation();
	}
}

void UDesGameplayAbilityPlayerInscribe::OnTimeout(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	EndAbilityCurrent();
}

void UDesGameplayAbilityPlayerInscribe::OnInputRelease(float TimeHeld)
{
	EndAbilityCurrent();
}
