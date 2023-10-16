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
	TSharedPtr<SDesItemWidget> ItemWidget;
	SCanvas::FSlot* ItemWidgetSlot{};
	bool bIsItemMoveActive{};

public:
	SLATE_BEGIN_ARGS(SDesItemLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void HandlePointer(const FPointerEvent& PointerEvent) const;
	void BeginItemMove(const FDesItemWidgetData& ItemWidgetData, FVector2D ScreenSpacePosition);
	void EndItemMove();
	bool IsItemMoveActive() const;
};
