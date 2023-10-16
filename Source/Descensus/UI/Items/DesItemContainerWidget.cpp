#include "DesItemContainerWidget.h"

#include "DesItemLayer.h"
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

void UDesItemContainerWidget::SetItemLayer(UDesItemLayer* InItemLayer)
{
	ItemLayer = MakeWeakObjectPtr(InItemLayer);
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
				Widget->AddItem(Entry.Position, GetItemWidgetData(ItemInstance));
			}
		}
	});

	Widget->SetGridSize(ItemContainerComponent->GridSize);
	Widget->OnItemContainerClickedDelegate.BindUObject(this, &ThisClass::OnItemContainerClicked);
}

FReply UDesItemContainerWidget::OnItemContainerClicked(const FGeometry& Geometry, const FPointerEvent& MouseEvent,
                                                       const FIntVector2& Coords) const
{
	if (const auto ItemInstance = ItemContainerComponent->GetItemInstance(Coords))
	{
		if (ItemLayer->IsItemMoveActive())
		{
			DES_LOG_STR("TO SWAP")
			ItemLayer->EndItemMove();
		}
		else
		{
			ItemLayer->BeginItemMove(ItemContainerComponent.Get(), ItemInstance, GetItemWidgetData(ItemInstance),
			                         MouseEvent.GetScreenSpacePosition());
		}
	}
	else
	{
		DES_LOG_STR("TO MOVE")
		if (ItemLayer->IsItemMoveActive())
		{
			ItemContainerComponent->ServerMoveItem(ItemLayer->GetItemToMove(), Coords);
			ItemLayer->EndItemMove();
		}
	}
	return FReply::Handled();
}

FDesItemWidgetData UDesItemContainerWidget::GetItemWidgetData(const UDesItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		const auto ItemData = ItemInstance->GetItemData();

		return FDesItemWidgetData{
			ItemData->Size, ItemInstance->Quantity,
			ItemData->MaxQuantity, &ItemData->IconBrush
		};
	}
	return FDesItemWidgetData{};
}

const FText UDesItemContainerWidget::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}

#undef LOCTEXT_NAMESPACE
