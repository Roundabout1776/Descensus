#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DesWaterVolume.generated.h"

UCLASS()
class DESCENSUS_API ADesWaterVolume : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category="Descensus|Water")
	TObjectPtr<UChildActorComponent> PostProcessVolumeChildActor;

	UPROPERTY(VisibleDefaultsOnly, Category="Descensus|Water")
	TObjectPtr<UChildActorComponent> PhysicsVolumeChildActor;

protected:
	virtual void BeginPlay() override;

public:
	ADesWaterVolume();
	virtual void Tick(float DeltaTime) override;
	virtual void PostRegisterAllComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Descensus|Water")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
