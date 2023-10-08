#include "Magic/DesGesture.h"

#include "DesConstants.h"
#include "Magic/DesStrokePoint.h"

bool FDesGesture::UpdateFromPointsRaw()
{
	if (PointsRaw.Num() < 5)
	{
		return false;
	}
	/** @TODO fix */
	Points = Resample(PointsRaw);
	if (Points.Num() != RR_CLOUD_SIZE)
	{
		return false;
	}
	Scale(Points);
	TranslateTo(Points, FVector2D::ZeroVector);
	ComputeCoordsInt();
	ComputeLUT();
	return true;
}

void FDesGesture::ComputeLUT()
{
	LUT.SetNumZeroed(RR_LOOKUP_TABLE_SIZE * RR_LOOKUP_TABLE_SIZE);
	for (int X = 0; X < RR_LOOKUP_TABLE_SIZE; ++X)
	{
		for (int Y = 0; Y < RR_LOOKUP_TABLE_SIZE; ++Y)
		{
			int Min = TNumericLimits<int>::Max();
			int Index = -1;
			for (int I = 0; I < Points.Num(); ++I)
			{
				const int Row = FMath::RoundToInt(
					Points[I].CoordsInt.X / static_cast<double>(RR_LOOKUP_TABLE_SCALE_FACTOR));
				const int Col = FMath::RoundToInt(
					Points[I].CoordsInt.Y / static_cast<double>(RR_LOOKUP_TABLE_SCALE_FACTOR));
				if (const auto DistanceSquared = ((Row - X) * (Row - X)) + ((Col - Y) * (Col - Y)); DistanceSquared <
					Min)
				{
					Min = DistanceSquared;
					Index = I;
				}
			}
			LUT[X + Y * RR_LOOKUP_TABLE_SIZE] = Index;
		}
	}
}

void FDesGesture::ComputeCoordsInt()
{
	for (auto& Point : Points)
	{
		Point.CoordsInt.X = FMath::RoundToInt((Point.Coords.X + 1.0) / 2.0 * (RR_MAX_INT_COORDINATES - 1));
		Point.CoordsInt.Y = FMath::RoundToInt((Point.Coords.Y + 1.0) / 2.0 * (RR_MAX_INT_COORDINATES - 1));
	}
}

double FDesGesture::GetPathLength(const TArray<FDesStrokePoint>& Points)
{
	double Length = 0;
	for (int i = 1; i < Points.Num(); i++)
	{
		if (Points[i].StrokeId == Points[i - 1].StrokeId)
		{
			Length += FVector2D::Distance(Points[i - 1].Coords, Points[i].Coords);
		}
	}
	return Length;
}

TArray<FDesStrokePoint> FDesGesture::Resample(TArray<FDesStrokePoint>& OldPoints)
{
	TArray<FDesStrokePoint> PointsResampled;
	PointsResampled.Add(OldPoints[0]);

	const double Step = GetPathLength(OldPoints) / (RR_CLOUD_SIZE - 1);
	if (FMath::IsNearlyZero(Step))
	{
		return PointsResampled;
	}
	double AccumulatedDistance = 0.0;
	for (int Index = 1; Index < OldPoints.Num(); ++Index)
	{
		if (OldPoints[Index].StrokeId == OldPoints[Index - 1].StrokeId)
		{
			if (const auto Distance = FVector2D::Distance(OldPoints[Index - 1].Coords, OldPoints[Index].Coords);
				AccumulatedDistance + Distance >= Step)
			{
				auto Alpha = FMath::Min(FMath::Max((Step - AccumulatedDistance) / Distance, 0.0), 1.0);
				if (FGenericPlatformMath::IsNaN(Alpha))
				{
					Alpha = 0.5;
				}

				auto NewPoint = FDesStrokePoint(
					FVector2D(
						OldPoints[Index - 1].Coords.X + Alpha * (OldPoints[Index].Coords.X - OldPoints[Index - 1].Coords
							.X),
						OldPoints[Index - 1].Coords.Y + Alpha * (OldPoints[Index].Coords.Y - OldPoints[Index - 1].Coords
							.Y)
					), OldPoints[Index].StrokeId);
				PointsResampled.Add(NewPoint);
				OldPoints.Insert(NewPoint, Index);
				AccumulatedDistance = 0;
			}
			else
			{
				AccumulatedDistance += Distance;
			}
		}
	}

	if (PointsResampled.Num() == RR_CLOUD_SIZE - 1)
	{
		const auto LastPoint = PointsResampled.Last();
		PointsResampled.Add(LastPoint);
	}

	return PointsResampled;
}

void FDesGesture::Scale(TArray<FDesStrokePoint>& Points)
{
	double MinX = TNumericLimits<double>::Max();
	double MinY = TNumericLimits<double>::Max();
	double MaxX = TNumericLimits<double>::Min();
	double MaxY = TNumericLimits<double>::Min();

	for (int I = 0; I < Points.Num(); ++I)
	{
		MinX = FMath::Min(MinX, Points[I].Coords.X);
		MinY = FMath::Min(MinY, Points[I].Coords.Y);
		MaxX = FMath::Max(MaxX, Points[I].Coords.X);
		MaxY = FMath::Max(MaxY, Points[I].Coords.Y);
	}

	const double Scale = FMath::Max(MaxX - MinX, MaxY - MinY);
	for (auto& Point : Points)
	{
		Point.Coords.X = (Point.Coords.X - MinX) / Scale;
		Point.Coords.Y = (Point.Coords.Y - MinY) / Scale;
	}
}

FVector2D FDesGesture::GetCentroid(const TArray<FDesStrokePoint>& Points)
{
	double Cx = 0;
	double Cy = 0;
	for (const auto& Point : Points)
	{
		Cx += Point.Coords.X;
		Cy += Point.Coords.Y;
	}

	return FVector2D(Cx / Points.Num(), Cy / Points.Num());
}

void FDesGesture::TranslateTo(TArray<FDesStrokePoint>& Points, const FVector2D& InPoint)
{
	const auto Centroid = GetCentroid(Points);
	for (auto& Point : Points)
	{
		Point.Coords.X += InPoint.X - Centroid.X;
		Point.Coords.Y += InPoint.Y - Centroid.Y;
	}
}
