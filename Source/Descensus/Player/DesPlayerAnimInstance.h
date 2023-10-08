#pragma once

#include "CoreMinimal.h"
#include "Character/Humanoid/DesHumanoidAnimInstance.h"
#include "DesPlayerAnimInstance.generated.h"

class ADesPlayerCharacter;

UCLASS()
class DESCENSUS_API UDesPlayerAnimInstance : public UDesHumanoidAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TObjectPtr<ADesPlayerCharacter> PlayerCharacter;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	uint8 bFirstPerson : 1;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void InitializeForCharacter(ADesCharacter* InCharacter) override;
};
