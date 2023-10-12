#include "SDesItemContainerWidget.h"

#include "DesLogging.h"
#include "DesWidgetStyleCatalog.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Engine/Texture2D.h"
#include "UI/DesStyle.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemContainerWidget::Construct(const FArguments& InArgs)
{
	GridSize = InArgs._GridSize;

	const auto Style = FDesStyle::GetDefaultStyle();

	BackgroundBrush = &Style->TestBrush;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(GridSize.Y * Style->CellSize)
		.WidthOverride(GridSize.X * Style->CellSize)
		[
			SNew(SCanvas)
			+ SCanvas::Slot()
			  .Expose(TelegraphSlot)
			  .Size(Style->CellSize)
			[
				SAssignNew(Telegraph, SImage)
				.Visibility(EVisibility::Collapsed)
				.Image(&Style->ItemContainerTelegraphBrush)
			]
		]
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

	// const auto Offset = FVector2D(Style->Padding, Style->Padding);
	const auto Size = AllottedGeometry.GetLocalSize();
	// const auto Square = AllottedGeometry.MakeChild(Size, FSlateLayoutTransform(Offset))
	//                                     .ToPaintGeometry();

	const auto PaintGeometry = AllottedGeometry.ToPaintGeometry();

	static TArray<FVector2D> PointsTemp;

	for (int X = 0; X <= Size.X; X += Style->CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({static_cast<float>(X), 0});
		PointsTemp.Add({static_cast<float>(X), Size.Y});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, PaintGeometry, PointsTemp,
		                             ESlateDrawEffect::None, FLinearColor::White, false, 1.0f);
	}

	for (int Y = 0; Y <= Size.Y; Y += Style->CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({0, static_cast<float>(Y)});
		PointsTemp.Add({Size.X, static_cast<float>(Y)});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, PaintGeometry, PointsTemp,
		                             ESlateDrawEffect::None, FLinearColor::White, false, 1.0f);
	}

	return LayerId;
}

void SDesItemContainerWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);

}

void SDesItemContainerWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseLeave(MouseEvent);

	Telegraph->SetVisibility(EVisibility::Collapsed);
}

FReply SDesItemContainerWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D MouseLocal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	MouseLocal /= MyGeometry.Size;
	MouseLocal *= FVector2D(GridSize.X, GridSize.Y);
	const FIntVector2 Coords{static_cast<int>(MouseLocal.X), static_cast<int>(MouseLocal.Y)};
	const auto bIsTelegraphVisible = Coords.X >= 0 && Coords.X < GridSize.X && Coords.Y >= 0 && Coords.Y < GridSize.Y;
	Telegraph->SetVisibility(bIsTelegraphVisible ? EVisibility::Visible : EVisibility::Collapsed);
	if (bIsTelegraphVisible && TelegraphSlot != nullptr)
	{
		TelegraphSlot->SetPosition(FVector2D{
			Coords.X * (MyGeometry.Size.X / GridSize.X), Coords.Y * (MyGeometry.Size.Y / GridSize.Y)
		});
	}
	// DES_LOG_CSTR("ItemContainerPos", *MouseLocal.ToString())
	return SCompoundWidget::OnMouseMove(MyGeometry, MouseEvent);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
