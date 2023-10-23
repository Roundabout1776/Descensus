#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DesAbilityTask_PlayerGrab.generated.h"

class ADesPlayerCharacter;
class UCharacterMovementComponent;
class UPhysicsHandleComponent;

UCLASS()
class DESCENSUS_API UDesAbilityTask_PlayerGrab : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	UPROPERTY()
	TWeakObjectPtr<ADesPlayerCharacter> Character;

public:
	virtual void TickTask(float DeltaTime) override;

	static UDesAbilityTask_PlayerGrab* PlayerGrab(UGameplayAbility* OwningAbility,
	                                             ADesPlayerCharacter* InCharacter);
};
