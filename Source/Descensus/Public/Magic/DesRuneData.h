#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DesGesture.h"
#include "DesRuneData.generated.h"

UCLASS(BlueprintType)
class DESCENSUS_API UDesRuneData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	explicit UDesRuneData(const FObjectInitializer& ObjectInitializer);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayTag;

	UPROPERTY()
	TArray<FDesGesture> Templates;
};
