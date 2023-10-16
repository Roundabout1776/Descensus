#include "SDesItemLayer.h"

#include "DesStyle.h"
#include "SDesItemWidget.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemLayer::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(Canvas, SCanvas)
		+ SCanvas::Slot()
		.Expose(DragDropWidgetSlot)
		[
			SAssignNew(DragDropWidget, SDesItemWidget)
		]
	];
}

void SDesItemLayer::HandlePointer(const FPointerEvent& PointerEvent) const
{
	if (IsItemDragDropActive())
	{
		auto Position = this->GetTickSpaceGeometry().AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());
		Position -= DragDropOffset;
		DragDropWidgetSlot->SetPosition(Position);
	}
}

void SDesItemLayer::BeginItemDragDrop(const FDesItemWidgetData& ItemWidgetData, const FVector2D ScreenSpacePosition)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto Size = FVector2D(ItemWidgetData.Size.X * Style->CellSize,
	                            ItemWidgetData.Size.Y * Style->CellSize);
	DragDropWidgetSlot->SetSize(Size);
	DragDropOffset = Size / 2.0;
	DragDropWidget->SetDataAndMakeVisible(ItemWidgetData);

	auto Position = this->GetTickSpaceGeometry().AbsoluteToLocal(ScreenSpacePosition);
	Position -= DragDropOffset;
	DragDropWidgetSlot->SetPosition(Position);

	SetVisibility(EVisibility::HitTestInvisible);
	bIsItemDragDropActive = true;
}

void SDesItemLayer::EndItemDragDrop()
{
	SetVisibility(EVisibility::Collapsed);
	bIsItemDragDropActive = false;
}

bool SDesItemLayer::IsItemDragDropActive() const
{
	return bIsItemDragDropActive;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
