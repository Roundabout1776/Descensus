#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DesAbilityTaskPlayerGrab.generated.h"

class UCharacterMovementComponent;
class UPhysicsHandleComponent;
class ADesActor;

UCLASS()
class DESCENSUS_API UDesAbilityTaskPlayerGrab : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	UPROPERTY()
	TWeakObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> Character;

public:
	virtual void TickTask(float DeltaTime) override;
};
