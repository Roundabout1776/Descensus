#pragma once

#include "CoreMinimal.h"
#include "DesInscriptionSubsystem.generated.h"

class ASceneCapture2D;
class UDesRuneData;
struct FDesGesture;
struct FDesStrokePoint;

UCLASS()
class DESCENSUS_API UDesInscriptionSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	double GetClosestPointFromCloud(const FDesStrokePoint& Point, double Theta, const TArray<FDesStrokePoint>& Cloud,
	                                const TArray<double>& ThetaCloud, int& OutIndexMin) const;
	double CloudMatch(const FDesGesture& GestureA, const FDesGesture& GestureB, double MinSoFar) const;
	static double CloudDistance(const TArray<FDesStrokePoint>& PointsA,
	                            const TArray<FDesStrokePoint>& PointsB, int StartIndex, double MinSoFar);
	void ComputeLowerBound(const TArray<FDesStrokePoint>& PointsA, const TArray<FDesStrokePoint>& PointsB,
	                       const TArray<int>& LUT, int Step, TArray<double>& OutResult) const;

public:
	UFUNCTION(BlueprintCallable)
	UDesRuneData* Recognize(const FDesGesture& Gesture, const TArray<UDesRuneData*>& Runes, double& OutScore) const;
};
