#pragma once

#include "CoreMinimal.h"
#include "Character/DesCharacter.h"
#include "DesPlayerCharacter.generated.h"

class UPhysicsHandleComponent;
struct FGameplayAbilitySpecHandle;
struct FGameplayAbilitySpec;
struct FInputActionInstance;
class UDesGameplayAbilityPlayerInscribe;
class UDesInputConfig;
class UDesHeadBobComponent;
class UCameraComponent;
class UInputComponent;

UCLASS(Abstract)
class DESCENSUS_API ADesPlayerCharacter : public ADesCharacter
{
	GENERATED_BODY()

	FVector2D LookAxis2D;

protected:
	virtual void BeginPlay() override;

	void InputLookAxisTriggered(const FInputActionInstance& Instance);
	void InputMoveTriggered(const FInputActionInstance& Instance);

	/** These activate abilities from InputConfig */
	void InputAbilityPressed(const int32 InputID);
	void InputAbilityReleased(const int32 InputID);

	void SetFirstPersonMode(bool bFirstPerson) const;

public:
	ADesPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|PlayerCharacter")
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|PlayerCharacter")
	TObjectPtr<USkeletalMesh> RegularMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|PlayerCharacter")
	TObjectPtr<USkeletalMesh> FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|PlayerCharacter")
	TObjectPtr<USceneComponent> FirstPersonMeshSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|PlayerCharacter")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Descensus|PlayerCharacter")
	TObjectPtr<UDesHeadBobComponent> HeadBob;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Input")
	UDesInputConfig* InputConfig;
};
