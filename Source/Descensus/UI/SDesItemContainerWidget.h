#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCanvas.h"
#include "Widgets/SCompoundWidget.h"

class DESCENSUS_API SDesItemContainerWidget : public SCompoundWidget
{
	SCanvas::FSlot* TelegraphSlot;
	TSharedPtr<SImage> Telegraph;
	TSharedPtr<SCanvas> Canvas;
	const FSlateBrush* BackgroundBrush{};

public:
	SLATE_BEGIN_ARGS(SDesItemContainerWidget)
		{
		}

		SLATE_ARGUMENT(FIntVector, GridSize)

	SLATE_END_ARGS()

	FIntVector GridSize{1, 1, 0};

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void AddItem(FIntVector2 Position, FIntVector2 Size, const FSlateBrush* Texture);
};
