#pragma once
#include "Widgets/SCanvas.h"

struct FDesItemWidgetData;
class SDesItemWidget;
class SCanvas;

class DESCENSUS_API SDesItemLayer final : public SCompoundWidget
{
protected:
	FVector2D MousePositionLocal;
	FVector2D EjectedItemOffset;
	TSharedPtr<SCanvas> Canvas;
	TSharedPtr<SDesItemWidget> EjectedItemWidget;
	SCanvas::FSlot* EjectedItemWidgetSlot{};

public:
	SLATE_BEGIN_ARGS(SDesItemLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void UpdateEjectedItemPosition(const FVector2D& MousePosition, float DeltaTime);
	void BeginItemMove(const FDesItemWidgetData& ItemWidgetData);
	void EndItemMove() const;
};
