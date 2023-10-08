#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/DesGameplayAbilityWithData.h"
#include "DesGameplayAbilityPlayerGrab.generated.h"

class UGameplayCueNotify_Static;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_PlayerGrab)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Ability_PlayerGrab)

UCLASS()
class DESCENSUS_API UDesGameplayAbilityPlayerGrab : public UDesGameplayAbilityWithData
{
	GENERATED_BODY()

	TWeakObjectPtr<UPrimitiveComponent> CachedPrimitiveComponent;

protected:
	virtual void EndAbilityCleanup(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                               bool bWasCancelled) override;
	virtual void ActivateAbilityLocalPlayer(const FGameplayAbilitySpecHandle Handle,
	                                        const FGameplayAbilityActorInfo* ActorInfo,
	                                        const FGameplayAbilityActivationInfo ActivationInfo,
	                                        const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& LocalTargetDataHandle,
	                                           FGameplayTag GameplayTag) override;

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                FGameplayTagContainer* OptionalRelevantTags) const override;

	UDesGameplayAbilityPlayerGrab();
};
