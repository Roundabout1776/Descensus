#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"
#include "DesGameplayAbilityAttack.generated.h"

enum class EDesWeaponType : uint8;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_Attacking)

UCLASS()
class DESCENSUS_API UDesGameplayAbilityAttack : public UDesGameplayAbilityPrimaryBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	TMap<EDesWeaponType, TObjectPtr<UAnimMontage>> Montages;
	
	UFUNCTION()
	void OnMontageCompleted();
	
	UFUNCTION()
	void OnMontageCancelled();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
public:
	UDesGameplayAbilityAttack();
};
