#include "Character/DesCharacterAttributeSet.h"

#include "Net/UnrealNetwork.h"

FName UDesCharacterAttributeSet::AttributeSetName(TEXT("CharacterAttributeSet"));

#define IMPLEMENT_ATTRIBUTE_ONREP(ClassName, PropertyName) \
	void ClassName::OnRep_ ## PropertyName(const FGameplayAttributeData& Old ## PropertyName) \
	{ \
		GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, PropertyName, Old ## PropertyName); \
	}

IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, Health)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, MaxHealth)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, Mana)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, MaxMana)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, Stamina)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, MaxStamina)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, MovementSpeed)
IMPLEMENT_ATTRIBUTE_ONREP(UDesCharacterAttributeSet, MovementSpeedRunMultiplier)

void UDesCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UDesCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UDesCharacterAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UDesCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDesCharacterAttributeSet, MovementSpeedRunMultiplier, COND_None, REPNOTIFY_Always);
}
