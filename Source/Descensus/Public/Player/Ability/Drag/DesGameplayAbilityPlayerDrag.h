#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DesGameplayAbility.h"
#include "NativeGameplayTags.h"
#include "DesGameplayAbilityPlayerDrag.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_PlayerDrag)

UCLASS()
class DESCENSUS_API UDesGameplayAbilityPlayerDrag : public UDesGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	
protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UDesGameplayAbilityPlayerDrag();
};
