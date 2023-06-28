#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DesCharacterMovementComponent.generated.h"

class ADesCharacter;

UCLASS()
class DESCENSUS_API UDesCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FDesSavedMove_Character final : public FSavedMove_Character
	{
		using Super = FSavedMove_Character;

		uint8 bWantsToRunSaved : 1;

		virtual bool
		CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, const FVector& NewAccel,
		                        FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;

	public:
		FDesSavedMove_Character() = default;
	};

	class FDesNetworkPredictionData_Client_Character final : public FNetworkPredictionData_Client_Character
	{
	public:
		explicit FDesNetworkPredictionData_Client_Character(const UDesCharacterMovementComponent& ClientMovement);

		using Super = FNetworkPredictionData_Client_Character;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	uint8 bWantsToRunSafe : 1;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	UPROPERTY(Transient)
	TObjectPtr<ADesCharacter> DesCharacterOwner;

public:
	UDesCharacterMovementComponent();
	virtual void PostLoad() override;
	virtual float GetMaxSpeed() const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION()
	FORCEINLINE void SetRunning(bool bNewRunning) { bWantsToRunSafe = bNewRunning; };

	UFUNCTION(BlueprintPure)
	bool IsEligibleForRunning() const;
};
