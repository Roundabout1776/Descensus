#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/DesPlayerController.h"
#include "DesGameplayAbility.generated.h"

enum class EDesAbilityInputID : uint8;

UCLASS()
class DESCENSUS_API UDesGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	FGameplayTagContainer NonReplicatedActivationOwnedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	FGameplayTagContainer ReplicatedActivationOwnedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	FGameplayTagContainer LocalActivationOwnedTags;

	ADesPlayerController* GetDesPlayerControllerFromActorInfo() const
	{
		return (CurrentActorInfo ? Cast<ADesPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
	}

	ADesPlayerCharacter* GetDesPlayerCharacterFromActorInfo() const
	{
		return (CurrentActorInfo ? Cast<ADesPlayerCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
	}

	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                         const FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;
	/* NO BLUEPRINT SUPPORT FOR NOW!
	 * Activates server/local player routines depending on the context.
	 * Override the routines instead. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	/* Processes custom tag containers and calls EndAbilityCleanup when it's relevant.
	 * You should override EndAbilityCleanup in most cases. */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                               bool bWasCancelled);

	/* Always authority. */
	virtual void ActivateAbilityServer(const FGameplayAbilitySpecHandle Handle,
	                                   const FGameplayAbilityActorInfo* ActorInfo,
	                                   const FGameplayAbilityActivationInfo ActivationInfo,
	                                   const FGameplayEventData* TriggerEventData);
	/* Sometimes authority. */
	virtual void ActivateAbilityLocalPlayer(const FGameplayAbilitySpecHandle Handle,
	                                        const FGameplayAbilityActorInfo* ActorInfo,
	                                        const FGameplayAbilityActivationInfo ActivationInfo,
	                                        const FGameplayEventData* TriggerEventData);

	void EndAllTasks();

	void EndAbilityCurrent(bool bReplicateEndAbility = true, bool bWasCancelled = false);

public:
	UDesGameplayAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	EDesAbilityInputID AbilityInputID;
};
