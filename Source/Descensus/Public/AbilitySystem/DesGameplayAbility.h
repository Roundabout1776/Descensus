#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
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
	
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void EndAllTasks();

	void EndAbilityCurrent(bool bReplicateEndAbility = true, bool bWasCancelled = false);
	
public:
	UDesGameplayAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Ability")
	EDesAbilityInputID AbilityInputID;
};
