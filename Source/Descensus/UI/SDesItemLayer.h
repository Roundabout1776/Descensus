#pragma once
#include "Widgets/SCanvas.h"

struct FDesItemWidgetData;
class SDesItemWidget;
class SCanvas;

class DESCENSUS_API SDesItemLayer final : public SCompoundWidget
{
protected:
	FVector2D DragDropOffset;
	TSharedPtr<SCanvas> Canvas;
	TSharedPtr<SDesItemWidget> DragDropWidget;
	SCanvas::FSlot* DragDropWidgetSlot{};
	bool bIsItemDragDropActive{};

public:
	SLATE_BEGIN_ARGS(SDesItemLayer)
		{
		}

		SLATE_ATTRIBUTE(FVector2D, DragDropSize)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void HandlePointer(const FPointerEvent& PointerEvent) const;
	void BeginItemDragDrop(const FDesItemWidgetData& ItemWidgetData, FVector2D ScreenSpacePosition);
	void EndItemDragDrop();
	bool IsItemDragDropActive() const;
};
