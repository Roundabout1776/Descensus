#include "Player/DesPlayerAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "Character/Ability/DrawWeapon/DesGameplayAbilityDrawWeapon.h"
#include "Character/Ability/Inscribe/DesGameplayAbilityInscribe.h"
#include "Player/DesPlayerCharacter.h"

void UDesPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsInitializedForCharacter())
	{
	}
}

void UDesPlayerAnimInstance::InitializeForCharacter(ADesCharacter* InCharacter)
{
	Super::InitializeForCharacter(InCharacter);

	PlayerCharacter = Cast<ADesPlayerCharacter>(InCharacter);
	bFirstPerson = InCharacter->IsLocallyControlled();
}
