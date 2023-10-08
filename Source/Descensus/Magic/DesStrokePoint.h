#pragma once

#include "DesStrokePoint.generated.h"

USTRUCT()
struct DESCENSUS_API FDesStrokePoint
{
	GENERATED_USTRUCT_BODY()

	FDesStrokePoint()
	{
	}

	explicit FDesStrokePoint(const FVector2D& Coords, const int StrokeId = 0,
	                         const FIntVector2& CoordsInt = FIntVector2())
		: Coords(Coords),
		  StrokeId(StrokeId),
		  CoordsInt(CoordsInt)
	{
	}

	UPROPERTY()
	FVector2D Coords = FVector2D::ZeroVector;

	UPROPERTY()
	int StrokeId = 0;

	UPROPERTY()
	FIntVector2 CoordsInt = FIntVector2(0, 0);
};
