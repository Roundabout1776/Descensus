#pragma once

#include "CoreMinimal.h"
#include "Components/DesItemContainerComponent.h"
#include "GameFramework/PlayerController.h"
#include "DesPlayerController.generated.h"

class UDesItemContainerComponent;
class ADesContainerActor;
class ADesPlayerCharacter;
class UDesMetaComponent;
class UDesInputConfig;
class ADesHUD;
class ADesActor;
class UInputMappingContext;

UCLASS(Abstract)
class DESCENSUS_API ADesPlayerController : public APlayerController
{
	GENERATED_BODY()

	TWeakObjectPtr<ADesPlayerCharacter> PlayerCharacter;
	TWeakObjectPtr<ADesHUD> DesHUD;

	TWeakObjectPtr<UDesMetaComponent> MetaComponentUnderCursor;
	FIntPoint MousePosBeforeHiddenDueToCapture{};

	bool bIsLooking = false;

	void InputLookTriggered();
	void InputLookStarted();
	void InputLookCompleted();
	void InputPrimaryStarted();
	void InputPrimaryCompleted();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Input")
	TObjectPtr<UDesInputConfig> InputConfig;

	UPROPERTY(BlueprintReadOnly, Transient, ReplicatedUsing=OnRep_CurrentContainer)
	TObjectPtr<UDesItemContainerComponent> CurrentContainer;

	virtual void BeginPlay() override;

public:
	explicit ADesPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void AcknowledgePossession(APawn* InPawn) override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual bool ShouldShowMouseCursor() const override;

	UFUNCTION(BlueprintCallable)
	void GetMousePositionDetailed(FVector2D& Position, FVector2D& PositionNDC, FVector2D& PositionNormalized);

	FORCEINLINE bool IsLooking() const { return bIsLooking; }

	UPrimitiveComponent* GetCurrentClickablePrimitive(bool bForGrab) const;

	bool CheckIfCanInteractWithActor(const AActor* Actor);
	
	UFUNCTION(Server, Unreliable)
	void ServerOpenContainer(ADesContainerActor* ContainerActor);

	UFUNCTION()
	void OnRep_CurrentContainer() const;
};
