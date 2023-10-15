#include "DesItemContainerWidget.h"

#include "DesLogging.h"
#include "SDesItemContainerWidget.h"
#include "SDesItemWidget.h"
#include "Components/DesItemContainerComponent.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesItemContainerWidget::RebuildWidget()
{
	SAssignNew(Widget, SDesItemContainerWidget);
	return Widget.ToSharedRef();
}

void UDesItemContainerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UDesItemContainerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Widget.Reset();
}

void UDesItemContainerWidget::AttachToItemContainerComponent(UDesItemContainerComponent* InItemContainerComponent)
{
	check(InItemContainerComponent);

	ItemContainerComponent = MakeWeakObjectPtr(InItemContainerComponent);

	/* @TODO: Unbind! */
	ItemContainerComponent->OnAnyChangesDelegate.AddWeakLambda(this, [&](const TArray<FItemContainerEntry>& Items)
	{
		Widget->CollapseAllItems();
		for (auto& Entry : Items)
		{
			if (const auto ItemInstance = Entry.ItemInstance)
			{
				const auto ItemData = ItemInstance->GetItemData();
				Widget->AddItem(Entry.Position, FDesItemWidgetData{
					                ItemInstance->GetItemData()->Size, Entry.ItemInstance->Quantity,
					                ItemData->MaxQuantity, &ItemData->IconBrush
				                });
			}
		}
	});

	Widget->SetGridSize(ItemContainerComponent->GridSize);
	Widget->OnItemContainerClickedDelegate.BindUObject(this, &ThisClass::OnItemContainerClicked);
}

void UDesItemContainerWidget::OnItemContainerClicked(const FIntVector2& Coords) const
{
	// DES_LOG_INTVECTOR2("ClickedCoords", Coords)
	const auto ItemInstance = ItemContainerComponent->GetItemInstance(Coords);
	if (ItemInstance)
	{
		ItemContainerComponent->ServerDestroyItem(ItemInstance);
	}
}

const FText UDesItemContainerWidget::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}

#undef LOCTEXT_NAMESPACE
