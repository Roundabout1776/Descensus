#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DesMetaComponent.generated.h"

class UCreatureModel;
struct FDesTooltipData;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesMetaComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UDesMetaComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Descensus|Actor")
	FText Name;

	UFUNCTION()
	FDesTooltipData GetTooltipData() const;
};
