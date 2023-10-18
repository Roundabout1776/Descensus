#include "Player/DesPlayerState.h"

#include "DesPlayerAttributeSet.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"

ADesPlayerState::ADesPlayerState()
{
	NetUpdateFrequency = 60.0f;
	MinNetUpdateFrequency = 30.0f;

	AttributeSet = CreateDefaultSubobject<UDesPlayerAttributeSet>(UDesCharacterAttributeSet::AttributeSetName);

	ASC = CreateDefaultSubobject<UDesAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ADesPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UDesPlayerAttributeSet* ADesPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
