#pragma once

#include "CoreMinimal.h"
#include "DesGameplayTags.h"
#include "GameplayTagContainer.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Descensus|Meta")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Descensus|Meta", Replicated)
	FGameplayTagContainer Tags;

	UDesMetaComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool CanBeGrabbed() const { return Tags.HasTag(TAG_Meta_Grabbable) && !Tags.HasTag(TAG_Meta_Grabbed); }

	UFUNCTION()
	FDesTooltipData GetTooltipData() const;
};
