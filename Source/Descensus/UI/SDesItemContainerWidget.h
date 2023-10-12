#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCanvas.h"
#include "Widgets/SCompoundWidget.h"

class DESCENSUS_API SDesItemContainerWidget : public SCompoundWidget
{
	SCanvas::FSlot* TelegraphSlot;
	TSharedPtr<SImage> Telegraph;
	FIntVector2 GridSize{};
	const FSlateBrush* BackgroundBrush{};

public:
	SLATE_BEGIN_ARGS(SDesItemContainerWidget)
		{
		}

		SLATE_ARGUMENT(FIntVector2, GridSize)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
};
