#include "Player/DesPlayerAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "Character/Shared/Ability/DrawWeapon/DesGameplayAbilityDrawWeapon.h"
#include "Character/Shared/Ability/Inscribe/DesGameplayAbilityInscribe.h"
#include "Player/DesPlayerCharacter.h"

void UDesPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsInitializedForCharacter())
	{
		return;
	}
}

void UDesPlayerAnimInstance::InitializeForCharacter(ADesCharacter* InCharacter)
{
	Super::InitializeForCharacter(InCharacter);
	
	PlayerCharacter = Cast<ADesPlayerCharacter>(InCharacter);
	bFirstPerson = InCharacter->IsLocallyControlled();
}
