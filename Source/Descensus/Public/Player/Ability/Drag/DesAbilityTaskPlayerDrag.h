#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DesAbilityTaskPlayerDrag.generated.h"

class ADesActor;

UCLASS()
class DESCENSUS_API UDesAbilityTaskPlayerDrag : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY()
	TWeakObjectPtr<ADesActor> DragActor;

protected:
	virtual void Activate() override;

public:
	virtual void TickTask(float DeltaTime) override;
};
