#include "Character/Humanoid/DesHumanoidAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DesGameplayAbilityHandsBase.h"
#include "Character/DesCharacter.h"
#include "Character/Shared/Ability/DrawWeapon/DesGameplayAbilityDrawWeapon.h"
#include "Character/Shared/Ability/Inscribe/DesGameplayAbilityInscribe.h"

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

	bSomethingInHands = Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		TAG_Ability_Hands_Active);

	if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_Ability_DrawWeapon_Active))
	{
		EquipType = EHandsReadyType::Unarmed;
	}
	else if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_Ability_Inscribe_Active))
	{
		EquipType = EHandsReadyType::Inscribe;
	}
	else
	{
		EquipType = EHandsReadyType::None;
	}
}
