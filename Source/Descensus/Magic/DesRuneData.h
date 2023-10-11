#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DesGesture.h"
#include "DesPrimaryDataAsset.h"
#include "DesRuneData.generated.h"

UCLASS(BlueprintType)
class DESCENSUS_API UDesRuneData : public UDesPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Descensus|Rune")
	FGameplayTag GameplayTag;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Rune")
	TArray<FDesGesture> Templates;
	
	explicit UDesRuneData(const FObjectInitializer& ObjectInitializer);
	virtual FPrimaryAssetType GetPrimaryAssetType() const override;
};
