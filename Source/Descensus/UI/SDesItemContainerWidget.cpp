#include "SDesItemContainerWidget.h"

#include "DesWidgetStyleCatalog.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Engine/Texture2D.h"
#include "UI/DesStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemContainerWidget::Construct(const FArguments& InArgs)
{
	GridSize = InArgs._GridSize;

	const auto Style = FDesStyle::GetDefaultStyle();

	BackgroundBrush = &Style->TestBrush;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(Style->Padding * 2.0f + GridSize.Y * Style->CellSize)
		.WidthOverride(Style->Padding * 2.0f + GridSize.X * Style->CellSize)
	];
}

int32 SDesItemContainerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                       const FSlateRect& MyCullingRect,
                                       FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                       const FWidgetStyle& InWidgetStyle,
                                       bool bParentEnabled) const
{
	LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                   bParentEnabled);

	const auto Style = FDesStyle::GetDefaultStyle();

	const auto Offset = FVector2D(Style->Padding, Style->Padding);
	const auto Size = AllottedGeometry.GetLocalSize() - (Offset * 2.0f);
	const auto Square = AllottedGeometry.MakeChild(Size, FSlateLayoutTransform(Offset))
	                                    .ToPaintGeometry();

	static TArray<FVector2D> PointsTemp;

	for (int X = 0; X <= Size.X; X += Style->CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({static_cast<float>(X), 0});
		PointsTemp.Add({static_cast<float>(X), Size.Y});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, Square, PointsTemp,
		                             ESlateDrawEffect::None, FLinearColor::White, false, 1.0f);
	}

	for (int Y = 0; Y <= Size.Y; Y += Style->CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({0, static_cast<float>(Y)});
		PointsTemp.Add({Size.X, static_cast<float>(Y)});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, Square, PointsTemp,
		                             ESlateDrawEffect::None, FLinearColor::White, false, 1.0f);
	}

	return LayerId;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
