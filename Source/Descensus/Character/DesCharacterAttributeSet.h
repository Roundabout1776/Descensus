#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DesCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DESCENSUS_API UDesCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

protected:
	UDesCharacterAttributeSet() : Health(100.0f),
	                              MaxHealth(100.0f),
	                              Mana(100.0f),
	                              MaxMana(100.0f),
	                              Stamina(100.0f),
	                              MaxStamina(100.0f),
	                              MovementSpeed(435.0f),
	                              MovementSpeedRunMultiplier(1.62f)
	{
	};

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Descensus|Attributes", ReplicatedUsing = OnRep_MovementSpeedRunMultiplier)
	FGameplayAttributeData MovementSpeedRunMultiplier;

	UFUNCTION()
	virtual void OnRep_MovementSpeedRunMultiplier(const FGameplayAttributeData& OldMovementSpeedRunMultiplier);

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

public:
	static FName AttributeSetName;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, MovementSpeedRunMultiplier)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, MovementSpeed)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, MaxStamina)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UDesCharacterAttributeSet, Health);
};
