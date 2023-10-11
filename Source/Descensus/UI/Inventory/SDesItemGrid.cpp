﻿#include "SDesItemGrid.h"

#include "EnhancedInputSubsystems.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Engine/Texture2D.h"
#include "UI/DesStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemGrid::Construct(const FArguments& InArgs)
{
	GridSize = InArgs._GridSize;

	BackgroundBrush = FDesStyle::Get().GetBrush("/Brush/SB_Test");

	// const auto InputTextBlockStyle = FDesStyle::Get().GetWidgetStyle<FTextBlockStyle>("/Style/SWS_Default");

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(Padding * 2.0f + GridSize.Y * CellSize)
		.WidthOverride(Padding * 2.0f + GridSize.X * CellSize)
		[
			SNew(SBorder)
			.Padding(Padding)
			.BorderImage(FDesStyle::Get().GetBrush("/Brush/SB_CommonBox"))
		]
	];
}

int32 SDesItemGrid::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                            FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                            bool bParentEnabled) const
{
	LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                   bParentEnabled);

	const auto Offset = FVector2D(Padding, Padding);
	const auto Size = AllottedGeometry.GetLocalSize() - (Offset * 2.0f);
	const auto Square = AllottedGeometry.MakeChild(Size, FSlateLayoutTransform(Offset))
	                                    .ToPaintGeometry();

	static TArray<FVector2D> PointsTemp;

	for (int X = 0; X <= Size.X; X += CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({static_cast<float>(X), 0});
		PointsTemp.Add({static_cast<float>(X), Size.Y});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, Square, PointsTemp,
		                             ESlateDrawEffect::None, FLinearColor::White, false, 1.0f);
	}

	for (int Y = 0; Y <= Size.Y; Y += CellSize)
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