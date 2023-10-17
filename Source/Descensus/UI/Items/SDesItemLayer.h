#pragma once
#include "Widgets/SCanvas.h"

struct FDesItemWidgetData;
class SDesItemWidget;
class SCanvas;

class DESCENSUS_API SDesItemLayer final : public SCompoundWidget
{
protected:
	FVector2D PointerPosition;
	FVector2D EjectedItemOffset;
	TSharedPtr<SCanvas> Canvas;
	TSharedPtr<SDesItemWidget> ItemWidget;
	SCanvas::FSlot* ItemWidgetSlot{};

public:
	SLATE_BEGIN_ARGS(SDesItemLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void HandlePointer(const FPointerEvent& PointerEvent, float DeltaTime);
	void BeginItemMove(const FDesItemWidgetData& ItemWidgetData);
	void EndItemMove() const;
};
