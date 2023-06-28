#include "UI/DesStatusBar.h"

#include "Player/DesPlayerCharacter.h"
#include "Player/DesPlayerAttributeSet.h"
#include "UI/DesStatusBarResource.h"

void UDesStatusBar::OnAttributeChangedHandler(const FOnAttributeChangeData& Data)
{
	const auto PlayerCharacterModel = CastChecked<ADesPlayerCharacter>(GetOwningPlayerPawn());
	const auto AttributeSet = CastChecked<UDesCharacterAttributeSet>(PlayerCharacterModel->GetAttributeSet());

	if (Data.Attribute == UDesCharacterAttributeSet::GetHealthAttribute() || Data.Attribute ==
		UDesCharacterAttributeSet::GetMaxHealthAttribute())
	{
		HealthBar->SetValues(AttributeSet->GetHealth(),
		                     AttributeSet->GetMaxHealth());
	}
	else if (Data.Attribute == UDesCharacterAttributeSet::GetManaAttribute() || Data.Attribute ==
		UDesCharacterAttributeSet::GetMaxManaAttribute())

	{
		ManaBar->SetValues(AttributeSet->GetMana(),
		                   AttributeSet->GetMaxMana());
	}
	else if (Data.Attribute == UDesCharacterAttributeSet::GetStaminaAttribute() || Data.Attribute ==
		UDesCharacterAttributeSet::GetMaxStaminaAttribute())
	{
		StaminaBar->
			SetValues(AttributeSet->GetStamina(),
			          AttributeSet->GetMaxStamina());
	}
}

void UDesStatusBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto PlayerCharacter = CastChecked<ADesPlayerCharacter>(GetOwningPlayerPawn());
	const auto ACS = PlayerCharacter->GetAbilitySystemComponent();
	const auto AttributeSet = CastChecked<UDesCharacterAttributeSet>(PlayerCharacter->GetAttributeSet());

	if (ACS && AttributeSet)
	{
		ACS->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(
			this, &UDesStatusBar::OnAttributeChangedHandler);
		ACS->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(
			this, &UDesStatusBar::OnAttributeChangedHandler);

		ACS->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(
			this, &UDesStatusBar::OnAttributeChangedHandler);
		ACS->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).AddUObject(
			this, &UDesStatusBar::OnAttributeChangedHandler);

		ACS->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(
			this, &UDesStatusBar::OnAttributeChangedHandler);
		ACS->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(
			this, &UDesStatusBar::OnAttributeChangedHandler);

#define LOCTEXT_NAMESPACE "GameSystem"
		HealthBar->SetTextAndValues(LOCTEXT("Health", "Health: {0}/{1}"), AttributeSet->GetHealth(),
		                            AttributeSet->GetMaxHealth());
		ManaBar->SetTextAndValues(LOCTEXT("Mana", "Mana: {0}/{1}"), AttributeSet->GetMana(),
		                          AttributeSet->GetMaxMana());
		StaminaBar->
			SetTextAndValues(LOCTEXT("Stamina", "Stamina: {0}/{1}"), AttributeSet->GetStamina(),
			                 AttributeSet->GetMaxStamina());
#undef LOCTEXT_NAMESPACE
	}
}
