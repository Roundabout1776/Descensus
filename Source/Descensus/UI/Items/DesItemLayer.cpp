#include "DesItemLayer.h"

#include "SDesItemLayer.h"
#include "SDesItemWidget.h"
#include "Components/DesItemContainerComponent.h"

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

void UDesItemLayer::BeginItemMove(UDesItemContainerComponent* InContainerToMoveFrom, UDesItemInstance* InItemToMove,
	const FDesItemWidgetData& ItemWidgetData, const FVector2D ScreenSpacePosition)
{
	ContainerToMoveFrom = MakeWeakObjectPtr(InContainerToMoveFrom);
	ItemToMove = MakeWeakObjectPtr(InItemToMove);
	Widget->BeginItemMove(ItemWidgetData, ScreenSpacePosition);
	SetCursor(EMouseCursor::None);
}

void UDesItemLayer::EndItemMove()
{
	ContainerToMoveFrom.Reset();
	ItemToMove.Reset();
	Widget->EndItemMove();
}

bool UDesItemLayer::IsItemMoveActive() const
{
	return Widget && Widget->IsItemMoveActive();
}

void UDesItemLayer::HandlePointer(const FPointerEvent& PointerEvent) const
{
	Widget->HandlePointer(PointerEvent);
}

UDesItemInstance* UDesItemLayer::GetItemToMove() const
{
	return ItemToMove.Get();
}

UDesItemContainerComponent* UDesItemLayer::GetContainerToMoveFrom() const
{
	return ContainerToMoveFrom.Get();
}

const FText UDesItemLayer::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}

#undef LOCTEXT_NAMESPACE
