#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DesPrimaryDataAsset.generated.h"

UCLASS(Abstract, BlueprintType)
class DESCENSUS_API UDesPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Descensus|PrimaryDataAsset")
	FName UniqueId;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|PrimaryDataAsset")
	FText DisplayName;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual FPrimaryAssetType GetPrimaryAssetType() const;
};
