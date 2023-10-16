#include "SDesItemLayer.h"

#include "SDesItemWidget.h"
#include "UI/DesStyle.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemLayer::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(Canvas, SCanvas)
		+ SCanvas::Slot()
		.Expose(ItemWidgetSlot)
		[
			SAssignNew(ItemWidget, SDesItemWidget)
		]
	];
}

void SDesItemLayer::HandlePointer(const FPointerEvent& PointerEvent) const
{
	if (IsItemMoveActive())
	{
		auto Position = this->GetTickSpaceGeometry().AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());
		Position -= DragDropOffset;
		ItemWidgetSlot->SetPosition(Position);
	}
}

void SDesItemLayer::BeginItemMove(const FDesItemWidgetData& ItemWidgetData, const FVector2D ScreenSpacePosition)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto Size = FVector2D(ItemWidgetData.Size.X * Style->CellSize,
	                            ItemWidgetData.Size.Y * Style->CellSize);
	ItemWidgetSlot->SetSize(Size);
	DragDropOffset = Size / 2.0;
	ItemWidget->SetDataAndMakeVisible(ItemWidgetData);

	auto Position = this->GetTickSpaceGeometry().AbsoluteToLocal(ScreenSpacePosition);
	Position -= DragDropOffset;
	ItemWidgetSlot->SetPosition(Position);

	SetVisibility(EVisibility::HitTestInvisible);
	bIsItemMoveActive = true;
}

void SDesItemLayer::EndItemMove()
{
	SetVisibility(EVisibility::Collapsed);
	bIsItemMoveActive = false;
}

bool SDesItemLayer::IsItemMoveActive() const
{
	return bIsItemMoveActive;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
