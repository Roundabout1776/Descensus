#include "DesItemLayer.h"

#include "DesStyle.h"
#include "SDesItemLayer.h"
#include "SDesItemWidget.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesItemLayer::RebuildWidget()
{
	return SAssignNew(Widget, SDesItemLayer);
}

void UDesItemLayer::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Widget.Reset();
}

void UDesItemLayer::BeginItemDragDrop(const FDesItemWidgetData& ItemWidgetData,
                                      const FVector2D ScreenSpacePosition) const
{
	Widget->BeginItemDragDrop(ItemWidgetData, ScreenSpacePosition);
}

void UDesItemLayer::EndItemDragDrop() const
{
	Widget->EndItemDragDrop();
}

bool UDesItemLayer::IsItemDragDropActive() const
{
	return Widget && Widget->IsItemDragDropActive();
}

void UDesItemLayer::HandlePointer(const FPointerEvent& PointerEvent) const
{
	Widget->HandlePointer(PointerEvent);
}

const FText UDesItemLayer::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}

#undef LOCTEXT_NAMESPACE
