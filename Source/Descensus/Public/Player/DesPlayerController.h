#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DesPlayerController.generated.h"

class ADesPlayerCharacter;
class UDesMetaComponent;
class UDesInputConfig;
class ADesHUD;
class UInputMappingContext;

UCLASS(Abstract)
class DESCENSUS_API ADesPlayerController : public APlayerController
{
	GENERATED_BODY()

	TWeakObjectPtr<ADesPlayerCharacter> PlayerCharacter;
	TWeakObjectPtr<ADesHUD> DesHUD;

	TWeakObjectPtr<AActor> ActorUnderCursor;
	FIntPoint MousePosBeforeHiddenDueToCapture;

	bool bIsLooking;
	void InputLookTriggered();
	void InputLookStarted();
	void InputLookCompleted();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Input")
	UDesInputConfig* InputConfig;

public:
	ADesPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void AcknowledgePossession(APawn* InPawn) override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual bool ShouldShowMouseCursor() const override;

	UFUNCTION(BlueprintCallable)
	void GetMousePositionDetailed(FVector2D& Position, FVector2D& PositionNDC, FVector2D& PositionNormalized);

	FORCEINLINE bool IsLooking() const { return bIsLooking; };
};
