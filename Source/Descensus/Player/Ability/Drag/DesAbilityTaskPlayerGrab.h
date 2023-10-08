#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DesAbilityTaskPlayerGrab.generated.h"

class ADesPlayerCharacter;
class UCharacterMovementComponent;
class UPhysicsHandleComponent;

UCLASS()
class DESCENSUS_API UDesAbilityTaskPlayerGrab : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	TWeakObjectPtr<ADesPlayerCharacter> Character;

public:
	virtual void TickTask(float DeltaTime) override;
};
