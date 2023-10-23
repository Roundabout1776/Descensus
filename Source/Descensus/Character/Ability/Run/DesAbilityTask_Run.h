#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"
#include "DesAbilityTask_Run.generated.h"

class UDesCharacterMovementComponent;
class UMovementComponent;

DECLARE_DELEGATE_TwoParams(FRunDelegate, bool, float);

UCLASS()
class DESCENSUS_API UDesAbilityTask_Run : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	UPROPERTY()
	TWeakObjectPtr<UDesCharacterMovementComponent> CachedMovementComponent;

protected:
	virtual void Activate() override;

public:
	virtual void TickTask(float DeltaTime) override;

	FRunDelegate OnRun;
};
