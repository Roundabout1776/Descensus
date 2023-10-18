#include "DesItemLayer.h"

#include "DesItemContainerWidget.h"
#include "SDesItemLayer.h"
#include "SDesItemWidget.h"
#include "Player/DesInventoryComponent.h"
#include "Items/DesItemData.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesItemLayer::RebuildWidget()
{
	return SAssignNew(Widget, SDesItemLayer);
}

void UDesItemLayer::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Widget.Reset();

	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnAnyChangesDelegate.Remove(OnAnyChangesHandle);
		InventoryComponent->OnEjectedItemChanged.Remove(OnItemEjectedChangedHandle);
	}
}

void UDesItemLayer::ShowEjectedItem(const UDesItemInstance* InItem)
{
	Widget->ShowEjectedItem(UDesItemContainerWidget::GetItemWidgetData(InItem));
}

void UDesItemLayer::HideEjectedItem()
{
	Widget->HideEjectedItem();
}

const UDesItemInstance* UDesItemLayer::GetEjectedItem() const
{
	return InventoryComponent->GetEjectedItem();
}

void UDesItemLayer::UpdateEjectedItemPosition(const FVector2D& MousePosition, const float DeltaTime) const
{
	Widget->UpdateEjectedItemPosition(MousePosition, DeltaTime);
}

void UDesItemLayer::OnEjectedItemChanged(const UDesItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ShowEjectedItem(ItemInstance);
	}
	else
	{
		HideEjectedItem();
	}
}

void UDesItemLayer::OnAnyChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const
{
	if (const auto EjectedItem = GetEjectedItem())
	{
		Widget->UpdateEjectedItemQuantity(EjectedItem->GetQuantity(), EjectedItem->GetItemData()->MaxQuantity);
	}
}

void UDesItemLayer::AttachToInventory(UDesInventoryComponent* InInventoryComponent)
{
	InventoryComponent = MakeWeakObjectPtr(InInventoryComponent);

	OnItemEjectedChangedHandle = InventoryComponent->OnEjectedItemChanged.AddUObject(
		this, &ThisClass::OnEjectedItemChanged);
	OnAnyChangesHandle = InventoryComponent->OnAnyChangesDelegate.AddUObject(this, &ThisClass::OnAnyChanges);
}

const FText UDesItemLayer::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}

#undef LOCTEXT_NAMESPACE
