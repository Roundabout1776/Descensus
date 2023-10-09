#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "DesHumanoidAnimInstance.generated.h"

class ADesCharacter;

UENUM(BlueprintType)
enum class EHandsReadyType : uint8
{
	None,
	Unarmed,
	Inscribe
};

UCLASS()
class DESCENSUS_API UDesHumanoidAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	uint8 bInitializedForCharacter : 1;

	UPROPERTY(Transient)
	TWeakObjectPtr<ADesCharacter> Character;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float StrafeAxis;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float RunAxis;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FGameplayTagContainer Tags;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	uint8 bSomethingInHands : 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	EHandsReadyType EquipType;

	virtual void InitializeForCharacter(ADesCharacter* InCharacter);
	FORCEINLINE bool IsInitializedForCharacter() const { return bInitializedForCharacter; }

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
