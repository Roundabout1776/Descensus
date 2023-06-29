#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DesHeadBobComponent.generated.h"

class ADesPlayerCharacter;
class UCameraShakeBase;

UENUM(BlueprintType)
enum class EHeadBobMode : uint8
{
	Idle,
	Walk,
	Run
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesHeadBobComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<EHeadBobMode, UCameraShakeBase*> CameraShakeMap;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ADesPlayerCharacter> DesPlayerCharacterOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|HeadBob")
	TSubclassOf<UCameraShakeBase> IdleCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|HeadBob")
	TSubclassOf<UCameraShakeBase> WalkCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|HeadBob")
	TSubclassOf<UCameraShakeBase> RunCameraShakeClass;

public:
	UDesHeadBobComponent();
	virtual void PostLoad() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
