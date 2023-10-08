#include "Player/Ability/Drag/DesGameplayAbilityPlayerGrab.h"
#include "Player/Ability/Drag/DesAbilityTaskPlayerGrab.h"
#include "AbilitySystem/DesGameplayAbilityHandsBase.h"
#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "DesLogging.h"
#include "AbilitySystem/DesGameplayAbilityTargetDataTypes.h"
#include "Actor/DesMetaComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_PlayerDrag, "Ability.PlayerDrag")

UDesGameplayAbilityPlayerGrab::UDesGameplayAbilityPlayerGrab()
{
	AbilityTags.AddTag(TAG_Ability_PlayerDrag);
	AbilityTags.AddTag(TAG_Ability_Hands);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Hands);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Primary);
}

void UDesGameplayAbilityPlayerGrab::EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbilityCleanup(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (CachedPrimitiveComponent.IsValid())
	{
		const auto PhysicsHandle = GetDesPlayerCharacterFromActorInfo()->PhysicsHandle;
		PhysicsHandle->ReleaseComponent();

		if (const auto MetaComponent = CachedPrimitiveComponent->GetOwner()->GetComponentByClass<UDesMetaComponent>())
		{
			MetaComponent->Tags.RemoveTag(TAG_Meta_Grabbed);
		}
	}

	CachedPrimitiveComponent.Reset();
}

void UDesGameplayAbilityPlayerGrab::ActivateAbilityLocalPlayer(const FGameplayAbilitySpecHandle Handle,
                                                               const FGameplayAbilityActorInfo* ActorInfo,
                                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                                               const FGameplayEventData* TriggerEventData)
{
	FGameplayAbilityTargetDataPrimitiveComponent* TargetData = new FGameplayAbilityTargetDataPrimitiveComponent();

	if (const auto PC = GetDesPlayerControllerFromActorInfo())
	{
		if (const auto PrimitiveComponent = PC->GetCurrentClickablePrimitive(true))
		{
			TargetData->PrimitiveComponent = MakeWeakObjectPtr(PrimitiveComponent);
		}
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);
	NotifyTargetDataReady(TargetDataHandle, FGameplayTag());
}

void UDesGameplayAbilityPlayerGrab::ActivateAbilityWithTargetData(
	const FGameplayAbilityTargetDataHandle& LocalTargetDataHandle, FGameplayTag GameplayTag)
{
	const bool bIsAuthority = CurrentActorInfo->IsNetAuthority();

	const auto PrimitiveComponent = FGameplayAbilityTargetDataPrimitiveComponent::GetPrimitiveComponent(
		LocalTargetDataHandle);

	if (!IsValid(PrimitiveComponent))
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bIsAuthority);
		return;
	}

	const auto PhysicsHandle = GetDesPlayerCharacterFromActorInfo()->PhysicsHandle;
	PhysicsHandle->GrabComponentAtLocationWithRotation(PrimitiveComponent,
	                                                   NAME_None,
	                                                   PrimitiveComponent->GetComponentLocation(),
	                                                   PrimitiveComponent->GetComponentRotation());

	CachedPrimitiveComponent = MakeWeakObjectPtr(PrimitiveComponent);

	if (const auto MetaComponent = CachedPrimitiveComponent->GetOwner()->GetComponentByClass<UDesMetaComponent>())
	{
		MetaComponent->Tags.AddTag(TAG_Meta_Grabbed);
	}

	const auto GrabTask = UAbilityTask::NewAbilityTask<UDesAbilityTaskPlayerGrab>(this);
	GrabTask->Character = MakeWeakObjectPtr(GetDesPlayerCharacterFromActorInfo());
	GrabTask->ReadyForActivation();
}

bool UDesGameplayAbilityPlayerGrab::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayTagContainer* SourceTags,
                                                       const FGameplayTagContainer* TargetTags,
                                                       FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
