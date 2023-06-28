#include "Magic/DesInscriptionSubsystem.h"

#include "DesConstants.h"
#include "Magic/DesRuneData.h"
#include "Magic/DesStrokePoint.h"

double UDesInscriptionSubsystem::GetClosestPointFromCloud(const FDesStrokePoint& Point, const double Theta,
                                                          const TArray<FDesStrokePoint>& Cloud,
                                                          const TArray<double>& ThetaCloud, int& OutIndexMin) const
{
	double Min = TNumericLimits<double>::Max();

	OutIndexMin = -1;

	for (int I = 0; I < Cloud.Num(); ++I)
	{
		if (const double DistanceSquared = FMath::Sqrt(
				FVector2D::DistSquared(Point.Coords, Cloud[I].Coords) + (Theta - ThetaCloud[I]) * (Theta - ThetaCloud[
					I]));
			DistanceSquared < Min)
		{
			Min = DistanceSquared;
			OutIndexMin = I;
		}
	}

	return Min;
}

double UDesInscriptionSubsystem::CloudMatch(const FDesGesture& GestureA,
                                            const FDesGesture& GestureB, double MinSoFar) const
{
	const int Num = GestureA.Points.Num();
	const int Step = FMath::FloorToInt(FMath::Pow(Num, 0.5));

	static TArray<double> LowerBoundA;
	ComputeLowerBound(GestureA.Points, GestureB.Points, GestureB.LUT, Step, LowerBoundA);
	static TArray<double> LowerBoundB;
	ComputeLowerBound(GestureB.Points, GestureA.Points, GestureA.LUT, Step, LowerBoundB);

	for (int I = 0, IndexLowerBound = 0; I < Num; I += Step, IndexLowerBound++)
	{
		if (LowerBoundA[IndexLowerBound] < MinSoFar)
		{
			MinSoFar = FMath::Min(
				MinSoFar, CloudDistance(GestureA.Points, GestureB.Points, I, MinSoFar));
		}
		if (LowerBoundB[IndexLowerBound] < MinSoFar)
		{
			MinSoFar = FMath::Min(
				MinSoFar, CloudDistance(GestureB.Points, GestureA.Points, I, MinSoFar));
		}
	}

	return MinSoFar;
}

double UDesInscriptionSubsystem::CloudDistance(const TArray<FDesStrokePoint>& PointsA,
                                               const TArray<FDesStrokePoint>& PointsB, const int StartIndex,
                                               const double MinSoFar)
{
	const int Num = PointsA.Num();
	static TArray<int> IndicesNotMatched;
	IndicesNotMatched.SetNum(Num, false);
	for (int J = 0; J < Num; ++J)
	{
		IndicesNotMatched[J] = J;
	}

	double Sum = 0.0;
	int I = StartIndex;
	int Weight = Num;

	do
	{
		int Index = -1;
		double MinDistance = TNumericLimits<double>::Max();
		for (int J = 0; J < IndicesNotMatched.Num(); ++J)
		{
			if (const double Distance = FVector2D::DistSquared(PointsA[I].Coords, PointsB[IndicesNotMatched[J]].Coords);
				Distance < MinDistance)
			{
				MinDistance = Distance;
				Index = J;
			}
		}
		IndicesNotMatched.RemoveAt(Index);
		Sum += Weight * MinDistance;

		if (Sum >= MinSoFar)
		{
			return Sum;
		}

		Weight--;
		I = (I + 1) % Num;
	}
	while (I != StartIndex);

	return Sum;
}

UDesRuneData* UDesInscriptionSubsystem::Recognize(const FDesGesture& Gesture, const TArray<UDesRuneData*>& Runes,
                                                  double& OutScore) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_RuneRecognition_Recognize);

	OutScore = TNumericLimits<double>::Max();
	UDesRuneData* Result = nullptr;

	for (const auto Rune : Runes)
	{
		for (const auto& Template : Rune->Templates)
		{
			const auto D = CloudMatch(Gesture, Template, OutScore);
			if (D < OutScore)
			{
				OutScore = D;
				Result = Rune;
			}
		}
	}

	OutScore = OutScore > 1.0 ? 1.0 / OutScore : 1.0;

	return Result;
}

void UDesInscriptionSubsystem::ComputeLowerBound(const TArray<FDesStrokePoint>& PointsA,
                                                 const TArray<FDesStrokePoint>& PointsB,
                                                 const TArray<int>& LUT, const int Step,
                                                 TArray<double>& OutResult) const
{
	const int Num = PointsA.Num();
	OutResult.SetNumZeroed(FMath::FloorToInt(Num / static_cast<double>(Step)) + 1, false);
	OutResult[0] = 0.0;
	static TArray<double> SAT;
	SAT.SetNumZeroed(Num, false);

	for (int I = 0; I < Num; ++I)
	{
		const int LookupTableX = FMath::RoundToInt(
			PointsA[I].CoordsInt.X / static_cast<double>(RR_LOOKUP_TABLE_SCALE_FACTOR));
		const int LookupTableY = FMath::RoundToInt(
			PointsA[I].CoordsInt.Y / static_cast<double>(RR_LOOKUP_TABLE_SCALE_FACTOR));
		const int Index = LUT[LookupTableX + LookupTableY * RR_LOOKUP_TABLE_SIZE];
		const double DistanceSquared = FVector2D::DistSquared(PointsA[I].Coords, PointsB[Index].Coords);
		SAT[I] = I == 0 ? DistanceSquared : SAT[I - 1] + DistanceSquared;
		OutResult[0] += (Num - I) * DistanceSquared;
	}

	for (int I = Step, IndexLowerBound = 1; I < Num; I += Step, IndexLowerBound++)
	{
		OutResult[IndexLowerBound] = OutResult[0] + I * SAT[Num - 1] - Num * SAT[I - 1];
	}
}
