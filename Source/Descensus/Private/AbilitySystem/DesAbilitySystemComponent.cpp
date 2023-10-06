#include "AbilitySystem/DesAbilitySystemComponent.h"

static TArray<FGameplayAbilitySpec*> TemporaryAbilities;

void UDesAbilitySystemComponent::PressAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->IsA(AbilityClass))
		{
			PressAbilitySpec(Spec);
		}
	}
}

void UDesAbilitySystemComponent::ReleaseAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->IsA(AbilityClass))
		{
			ReleaseAbilitySpec(Spec);
		}
	}
}

void UDesAbilitySystemComponent::PressAbilitySpec(FGameplayAbilitySpec& Spec)
{
	Spec.InputPressed = true;
	if (Spec.IsActive())
	{
		if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			ServerSetInputPressed(Spec.Handle);
		}

		AbilitySpecInputPressed(Spec);

		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle,
		                      Spec.ActivationInfo.GetActivationPredictionKey());
	}
	else
	{
		// Ability is not active, so try to activate it
		TryActivateAbility(Spec.Handle);
	}
}

void UDesAbilitySystemComponent::ReleaseAbilitySpec(FGameplayAbilitySpec& Spec)
{
	Spec.InputPressed = false;
	if (Spec.IsActive())
	{
		if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
		{
			ServerSetInputReleased(Spec.Handle);
		}

		AbilitySpecInputReleased(Spec);

		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle,
		                      Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UDesAbilitySystemComponent::PressAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bPressOnlyActive)
{
	TemporaryAbilities.Empty();
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, TemporaryAbilities);
	ABILITYLIST_SCOPE_LOCK();
	for (const auto Spec : TemporaryAbilities)
	{
		if (Spec && (!bPressOnlyActive || Spec->
			IsActive()))
		{
			PressAbilitySpec(*Spec);
		}
	}
}

void UDesAbilitySystemComponent::ReleaseAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bReleaseOnlyActive)
{
	TemporaryAbilities.Empty();
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, TemporaryAbilities, false);
	ABILITYLIST_SCOPE_LOCK();
	for (const auto Spec : TemporaryAbilities)
	{
		if (Spec && (!bReleaseOnlyActive || Spec->
			IsActive()))
		{
			ReleaseAbilitySpec(*Spec);
		}
	}
}
