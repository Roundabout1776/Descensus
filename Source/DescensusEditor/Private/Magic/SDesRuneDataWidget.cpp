#include "Magic/SDesRuneDataWidget.h"

#include "DesConstants.h"
#include "Editor.h"
#include "Fonts/FontMeasure.h"
#include "Magic/DesStrokePoint.h"

void SDesRuneDataWidget::Construct(const FArguments& InArgs)
{
	PointsRaw = InArgs._PointsRaw;
	Points = InArgs._Points;
	OnResolutionChangedDelegate = InArgs._OnResolutionChangedDelegate;
	OnAddPointDelegate = InArgs._OnAddPointDelegate;
}

int32 SDesRuneDataWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Plot background
	const auto Size = AllottedGeometry.Size.GetMin();
	const auto OffsetX = AllottedGeometry.Size.X > AllottedGeometry.Size.Y
		                     ? (AllottedGeometry.Size.X - AllottedGeometry.Size.Y) / 2.0
		                     : 0.0;
	const auto OffsetY = AllottedGeometry.Size.Y > AllottedGeometry.Size.X
		                     ? (AllottedGeometry.Size.Y - AllottedGeometry.Size.X) / 2.0
		                     : 0.0;
	const auto Offset = FVector2D(OffsetX, OffsetY);
	const auto Square = AllottedGeometry.MakeChild(FVector2D(Size, Size), FSlateLayoutTransform(Offset))
	                                    .ToPaintGeometry();
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Square, &BackgroundBrush, ESlateDrawEffect::None,
	                           FLinearColor(0.1f, 0.05f, 0.075f));

	// Grid lines
	auto GridNumbersStyle = FAppStyle::Get().GetFontStyle("CurveEd.InfoFont");
	GridNumbersStyle.Size = 6;
	const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->
		GetFontMeasureService();

	static TArray<FVector2D> PointsTemp;

	for (int X = 0; X <= GridSize; ++X)
	{
		const auto NormalizedX = static_cast<float>(X) / GridSize;
		const auto RealX = NormalizedX * Size;
		const auto Thickness = FMath::IsNearlyEqual(NormalizedX, 0.5f) ? 2.0f : 1.0f;
		const auto Color = FMath::IsNearlyEqual(NormalizedX, 0.5f)
			                   ? FLinearColor::White
			                   : FLinearColor(0.7f, 0.7f, 0.7f);

		PointsTemp.Add(FVector2D(RealX, 0) + Offset);
		PointsTemp.Add(FVector2D(RealX, Size) + Offset);

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), PointsTemp,
		                             ESlateDrawEffect::None, Color, false, Thickness);

		if (X % 4 == 0)
		{
			FString TimeStr = FString::Printf(TEXT("%.2f"), NormalizedX - 0.5);
			const FVector2D DrawSize = FontMeasureService->Measure(TimeStr, GridNumbersStyle);
			FSlateDrawElement::MakeText(OutDrawElements, LayerId,
			                            AllottedGeometry.MakeChild(
				                                            FVector2D(1.0, 1.0),
				                                            FSlateLayoutTransform(
					                                            PointsTemp[0] - FVector2D(
						                                            DrawSize.X / 2.0, DrawSize.Y)))
			                                            .ToPaintGeometry(),
			                            TimeStr,
			                            GridNumbersStyle, ESlateDrawEffect::None,
			                            FLinearColor::White);

			FSlateDrawElement::MakeText(OutDrawElements, LayerId,
			                            AllottedGeometry.MakeChild(
				                                            FVector2D(1.0, 1.0),
				                                            FSlateLayoutTransform(
					                                            PointsTemp[1] - FVector2D(
						                                            DrawSize.X / 2.0, DrawSize.Y)))
			                                            .ToPaintGeometry(),
			                            TimeStr,
			                            GridNumbersStyle, ESlateDrawEffect::None,
			                            FLinearColor::White);
		}

		PointsTemp.Empty();
	}

	for (int Y = 0; Y <= GridSize; ++Y)
	{
		const auto NormalizedY = static_cast<float>(Y) / GridSize;
		const auto RealY = NormalizedY * Size;
		const auto Thickness = FMath::IsNearlyEqual(NormalizedY, 0.5f) ? 2.0f : 1.0f;
		const auto Color = FMath::IsNearlyEqual(NormalizedY, 0.5f)
			                   ? FLinearColor::White
			                   : FLinearColor(0.7f, 0.7f, 0.7f);

		PointsTemp.Add(FVector2D(0, RealY) + Offset);
		PointsTemp.Add(FVector2D(Size, RealY) + Offset);

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), PointsTemp,
		                             ESlateDrawEffect::None, Color, false, Thickness);

		if (Y % 4 == 0 && Y > 0 && Y < GridSize)
		{
			FString TimeStr = FString::Printf(TEXT("%.2f"), NormalizedY - 0.5);
			const FVector2D DrawSize = FontMeasureService->Measure(TimeStr, GridNumbersStyle);
			FSlateDrawElement::MakeText(OutDrawElements, LayerId,
			                            AllottedGeometry.MakeChild(
				                                            FVector2D(1.0, 1.0),
				                                            FSlateLayoutTransform(
					                                            PointsTemp[0] - FVector2D(DrawSize.X, DrawSize.Y / 2.0))
			                                            )
			                                            .ToPaintGeometry(),
			                            TimeStr,
			                            GridNumbersStyle, ESlateDrawEffect::None,
			                            FLinearColor::White);

			FSlateDrawElement::MakeText(OutDrawElements, LayerId,
			                            AllottedGeometry.MakeChild(
				                                            FVector2D(1.0, 1.0),
				                                            FSlateLayoutTransform(PointsTemp[1] -
					                                            FVector2D(0.0, DrawSize.Y / 2.0)))
			                                            .ToPaintGeometry(),
			                            TimeStr,
			                            GridNumbersStyle, ESlateDrawEffect::None,
			                            FLinearColor::White);
		}

		PointsTemp.Empty();
	}

	if (const auto PointsEditorRef = PointsRaw.Get(); PointsEditorRef != nullptr && PointsEditorRef->Num() > 1)
	{
		for (int I = 0; I < PointsEditorRef->Num() - 1; ++I)
		{
			PointsTemp.Add(NormalizedToLocal(AllottedGeometry,
			                                 (*PointsEditorRef)[I].Coords / RR_MAX_INT_COORDINATES));
			PointsTemp.Add(NormalizedToLocal(AllottedGeometry,
			                                 (*PointsEditorRef)[I + 1].Coords / RR_MAX_INT_COORDINATES));
			auto Test = static_cast<float>(I) / static_cast<float>(
				                                         PointsEditorRef->Num());
			FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
			                             PointsTemp,
			                             ESlateDrawEffect::None, FLinearColor::Blue, false,
			                             FMath::Lerp(10.0f, 75.0f,
			                                         static_cast<float>(I) / static_cast<float>(
				                                         PointsEditorRef->Num())));
			PointsTemp.Empty();
		}
	}

	if (const auto PointsResampledRef = Points.Get(); PointsResampledRef != nullptr && PointsResampledRef->Num() > 1)
	{
		for (int I = 0; I < PointsResampledRef->Num() - 1; ++I)
		{
			PointsTemp.Add(NormalizedToLocal(AllottedGeometry,
			                                 (*PointsResampledRef)[I].Coords));
			PointsTemp.Add(NormalizedToLocal(AllottedGeometry,
			                                 (*PointsResampledRef)[I + 1].Coords));
			FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
			                             PointsTemp,
			                             ESlateDrawEffect::None, FLinearColor::Red, false,
			                             FMath::Lerp(5.0f, 25.0f,
			                                         static_cast<float>(I) / static_cast<float>(
				                                         PointsResampledRef->Num())));
		}
		PointsTemp.Empty();
	}

	if
	(HasMouseCapture())
	{
		const auto ResolutionFloat = 2.0f / static_cast<float>(GridSize);
		const auto CursorLocal = NormalizedToLocal(AllottedGeometry,
		                                           FVector2D(FMath::GridSnap(PointUnderCursorNormalized.X,
		                                                                     ResolutionFloat),
		                                                     FMath::GridSnap(PointUnderCursorNormalized.Y,
		                                                                     ResolutionFloat)));
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
		                           AllottedGeometry.MakeChild(
			                                           FVector2D::One() * 10.0,
			                                           FSlateLayoutTransform(CursorLocal - (FVector2D::One() * 5.0)))
		                                           .ToPaintGeometry(),
		                           FAppStyle::Get().GetDefaultBrush(),
		                           ESlateDrawEffect::None, FLinearColor::Red);
	}

	return
		LayerId;
}

FReply SDesRuneDataWidget::OnMouseButtonDown(const FGeometry& AllottedGeometry, const FPointerEvent& MouseEvent)
{
	if (GEditor && GEditor->CanTransact() && ensure(!GIsTransacting))
	{
		GEditor->BeginTransaction(TEXT(""), INVTEXT("Edit Rune Data"), nullptr);

		const FVector2D LocalPosition = AllottedGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		PointUnderCursorNormalized = LocalToNormalized(AllottedGeometry, LocalPosition);
	}
	return FReply::Handled().CaptureMouse(SharedThis(this));
}

FReply SDesRuneDataWidget::OnMouseButtonUp(const FGeometry& AllottedGeometry, const FPointerEvent& MouseEvent)
{
	if (GEditor && GEditor->IsTransactionActive())
	{
		GEditor->EndTransaction();
	}
	return FReply::Handled().ReleaseMouseCapture();
}

FReply SDesRuneDataWidget::OnMouseMove(const FGeometry& AllottedGeometry, const FPointerEvent& MouseEvent)
{
	if (!HasMouseCapture() || PointsRaw.Get() == nullptr)
	{
		return FReply::Unhandled();
	}

	const FVector2D LocalPosition = AllottedGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	PointUnderCursorNormalized = LocalToNormalized(AllottedGeometry, LocalPosition);

	if (const auto PointInt = NormalizedToInt(PointUnderCursorNormalized); !PointsRaw.Get()->FindByPredicate(
		[&PointInt](const FDesStrokePoint& TestPoint)
		{
			return TestPoint.CoordsInt == PointInt;
		}))
	{
		auto _ = OnAddPointDelegate.ExecuteIfBound(
			FDesStrokePoint(PointUnderCursorNormalized * RR_MAX_INT_COORDINATES, 0));
	}

	return FReply::Handled();
}

FVector2D SDesRuneDataWidget::NormalizedToLocal(const FGeometry& AllottedGeometry, FVector2D InNormalized)
{
	InNormalized += FVector2D::One() / 2.0;
	InNormalized = InNormalized.ClampAxes(0.0, 1.0);
	const auto Size = AllottedGeometry.Size.GetMin();
	InNormalized *= Size;
	if (AllottedGeometry.Size.X > AllottedGeometry.Size.Y)
	{
		InNormalized.X += (AllottedGeometry.Size.X - Size) / 2.0;
	}
	else
	{
		InNormalized.Y += (AllottedGeometry.Size.Y - Size) / 2.0;
	}

	return InNormalized;
}

FVector2D SDesRuneDataWidget::LocalToNormalized(const FGeometry& AllottedGeometry, FVector2D InLocal)
{
	const auto Size = AllottedGeometry.Size.GetMin();
	if (AllottedGeometry.Size.X > AllottedGeometry.Size.Y)
	{
		InLocal.X -= (AllottedGeometry.Size.X - Size) / 2.0;
	}
	else
	{
		InLocal.Y -= (AllottedGeometry.Size.Y - Size) / 2.0;
	}
	InLocal /= Size;
	InLocal = InLocal.ClampAxes(0.0, 1.0);
	InLocal -= FVector2D::One() / 2.0;
	return InLocal;
}

FIntVector2 SDesRuneDataWidget::NormalizedToInt(FVector2D InNormalized) const
{
	InNormalized *= GridSize;
	return FIntVector2(FMath::RoundToInt(InNormalized.X), FMath::RoundToInt(InNormalized.Y));
}

FVector2D SDesRuneDataWidget::IntToNormalized(const FIntVector2& InInt) const
{
	FVector2D Normalized;
	Normalized.X = static_cast<double>(InInt.X) / GridSize;
	Normalized.Y = static_cast<double>(InInt.Y) / GridSize;
	Normalized = Normalized.ClampAxes(0.0, 1.0);
	return Normalized;
}
