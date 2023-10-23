#include "Character/DesInscriptionComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/DesCharacter.h"
#include "Character/Ability/Inscribe/DesGameplayAbility_Inscribe.h"
#include "Net/UnrealNetwork.h"
#include "Player/Ability/Inscribe/DesGameplayAbility_PlayerInscribe.h"

UDesInscriptionComponent::UDesInscriptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDesInscriptionComponent::PostLoad()
{
	Super::PostLoad();

	DesCharacterOwner = GetOwner<ADesCharacter>();
}

void UDesInscriptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDesInscriptionComponent::ServerSetInscriptionOffset_Implementation(
	const FVector2D& InInscriptionOffset)
{
	InscriptionOffset = InInscriptionOffset;
}

void UDesInscriptionComponent::SetInscriptionOffset(
	const FVector2D& InInscriptionOffset)
{
	ServerSetInscriptionOffset(InInscriptionOffset);
	InscriptionOffset = InInscriptionOffset;
}

bool UDesInscriptionComponent::IsInscribing() const
{
	return DesCharacterOwner && DesCharacterOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		TAG_Ability_PlayerInscribe_Active);
}

void UDesInscriptionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UDesInscriptionComponent, InscriptionOffset, COND_SimulatedOnly)
}
