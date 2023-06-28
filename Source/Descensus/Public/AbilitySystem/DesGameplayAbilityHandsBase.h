#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/DesGameplayAbility.h"
#include "DesGameplayAbilityHandsBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Hands)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Hands_Active)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Hands_Active_Local)

UENUM(BlueprintType)
enum class EHandsAbilityState : uint8
{
	TransitionIn,
	Ready,
	TransitionOut,
	NotReady
};

UCLASS(Abstract)
class DESCENSUS_API UDesGameplayAbilityHandsBase : public UDesGameplayAbility
{
	GENERATED_BODY()

	EHandsAbilityState State = EHandsAbilityState::NotReady;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	TObjectPtr<UAnimMontage> ReadyMontage;

	UFUNCTION()
	void OnInputPress(float TimeWaited);
	
	UFUNCTION()
	virtual void OnReady();
	
public:
	UDesGameplayAbilityHandsBase();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateWhileReady(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       		const FGameplayAbilityActivationInfo ActivationInfo);

	FORCEINLINE EHandsAbilityState GetState() const { return State; }
};
