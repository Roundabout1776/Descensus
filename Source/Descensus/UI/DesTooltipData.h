#pragma once

#include "CoreMinimal.h"
#include "DesTooltipData.generated.h"

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesTooltipData
{
	GENERATED_USTRUCT_BODY()

	FDesTooltipData()
	{
	}

	FDesTooltipData(const FText& Name)
		: Name(Name)
	{
	}

	UPROPERTY()
	FText Name;

	UPROPERTY()
	TArray<FText> Content;
};
