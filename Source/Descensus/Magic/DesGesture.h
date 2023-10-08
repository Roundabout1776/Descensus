#pragma once

#include "DesStrokePoint.h"
#include "DesGesture.generated.h"

struct FDesStrokePoint;

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesGesture
{
	GENERATED_USTRUCT_BODY()

	FDesGesture()
	{
	}

	bool UpdateFromPointsRaw();

	UPROPERTY()
	TArray<FDesStrokePoint> Points;

	UPROPERTY()
	TArray<FDesStrokePoint> PointsRaw;

	UPROPERTY()
	TArray<int> LUT;

private:
	void ComputeLUT();
	void ComputeCoordsInt();

	static double GetPathLength(const TArray<FDesStrokePoint>& Points);
	static TArray<FDesStrokePoint> Resample(TArray<FDesStrokePoint>& OldPoints);
	static void Scale(TArray<FDesStrokePoint>& Points);
	static void TranslateTo(TArray<FDesStrokePoint>& Points, const FVector2D& InPoint);
	static FVector2D GetCentroid(const TArray<FDesStrokePoint>& Points);
};
