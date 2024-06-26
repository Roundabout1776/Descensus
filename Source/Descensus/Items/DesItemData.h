﻿#pragma once

#include "CoreMinimal.h"
#include "DesPrimaryDataAsset.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DesItemData.generated.h"

class ADesActor;

UCLASS()
class DESCENSUS_API UDesItemData : public UDesPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	FGameplayTagContainer Tags;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	FIntVector2 Size{1, 1};

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	int MaxQuantity{1};

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	TSoftClassPtr<ADesActor> ActorClass;

	// UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	// TSoftClassPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Item")
	FSlateBrush IconBrush;

	virtual FPrimaryAssetType GetPrimaryAssetType() const override;
};
