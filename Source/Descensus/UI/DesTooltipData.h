#pragma once

#include "CoreMinimal.h"
#include "DesTooltipData.generated.h"

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesTooltipData
{
	GENERATED_BODY()

	FDesTooltipData()
	{
	}

	FDesTooltipData(const FText& InHeader)
		: Header(InHeader)
	{
	}

	UPROPERTY()
	FText Header;

	UPROPERTY()
	FText Description;
};
