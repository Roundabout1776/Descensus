#include "DesItemLayer.h"

#include "DesItemContainerWidget.h"
#include "SDesItemLayer.h"
#include "SDesItemWidget.h"
#include "Components/DesInventoryComponent.h"

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
		InventoryComponent->OnEjectedItemChanged.Remove(OnItemEjectedChangedHandle);
	}
}

void UDesItemLayer::ShowEjectedItem(const UDesItemInstance* InItem)
{
	if (InItem)
	{
		bIsLocked = false;
	}
	if (bIsLocked)
		return;
	Widget->BeginItemMove(UDesItemContainerWidget::GetItemWidgetData(InItem));
}

void UDesItemLayer::HideEjectedItem()
{
	Widget->EndItemMove();
	bIsLocked = false;
}

const UDesItemInstance* UDesItemLayer::GetEjectedItem() const
{
	return InventoryComponent->EjectedItem;
}

bool UDesItemLayer::IsLocked() const
{
	return bIsLocked;
}

void UDesItemLayer::HandlePointer(const FPointerEvent& PointerEvent, const float DeltaTime) const
{
	Widget->HandlePointer(PointerEvent, DeltaTime);
}

void UDesItemLayer::OnEjectedItemChanged(const UDesItemInstance* DesItemInstance)
{
	if (DesItemInstance)
	{
		ShowEjectedItem(DesItemInstance);
	}
	else
	{
		HideEjectedItem();
	}
}

void UDesItemLayer::AttachToInventory(UDesInventoryComponent* InInventoryComponent)
{
	InventoryComponent = MakeWeakObjectPtr(InInventoryComponent);
	OnItemEjectedChangedHandle = InventoryComponent->OnEjectedItemChanged.AddUObject(
		this, &ThisClass::OnEjectedItemChanged);
}

const FText UDesItemLayer::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}

#undef LOCTEXT_NAMESPACE
