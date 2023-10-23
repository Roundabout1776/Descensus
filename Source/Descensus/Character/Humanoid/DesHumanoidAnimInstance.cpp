#include "Character/Humanoid/DesHumanoidAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "Character/Ability/DesGameplayAbilityHandsBase.h"
#include "Character/DesCharacter.h"
#include "Character/Ability/DrawWeapon/DesGameplayAbility_DrawWeapon.h"
#include "Character/Ability/Inscribe/DesGameplayAbility_Inscribe.h"

void UDesHumanoidAnimInstance::InitializeForCharacter(ADesCharacter* InCharacter)
{
	IsValidChecked(InCharacter);
	Character = MakeWeakObjectPtr(InCharacter);
	bInitializedForCharacter = true;
}

void UDesHumanoidAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character.IsValid())
	{
		return;
	}

	StrafeAxis = FVector::DotProduct(Character->GetActorRightVector(), Character->GetVelocity().GetSafeNormal());
	RunAxis = FVector::DotProduct(Character->GetActorForwardVector(), Character->GetVelocity().GetSafeNormal());

	const auto ASC = Character->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	ASC->GetOwnedGameplayTags(Tags);

	bSomethingInHands = ASC->HasMatchingGameplayTag(
		TAG_Ability_Hands_Active);

	if (ASC->HasMatchingGameplayTag(TAG_Ability_DrawWeapon_Active))
	{
		EquipType = EHandsReadyType::Unarmed;
	}
	else if (ASC->HasMatchingGameplayTag(TAG_Ability_Inscribe_Active))
	{
		EquipType = EHandsReadyType::Inscribe;
	}
	else
	{
		EquipType = EHandsReadyType::None;
	}
}
