#include "Character/Humanoid/DesHumanoidAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "Character/Ability/DesGameplayAbilityHandsBase.h"
#include "Character/DesCharacter.h"
#include "Character/Ability/DrawWeapon/DesGameplayAbilityDrawWeapon.h"
#include "Character/Ability/Inscribe/DesGameplayAbilityInscribe.h"

void UDesHumanoidAnimInstance::InitializeForCharacter(ADesCharacter* InCharacter)
{
	ensure(IsValid(InCharacter));
	Character = InCharacter;
	bInitializedForCharacter = true;
}

void UDesHumanoidAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsInitializedForCharacter())
	{
		return;
	}

	const auto ASC = Character->GetAbilitySystemComponent();
	if (!ASC)
		return;

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
