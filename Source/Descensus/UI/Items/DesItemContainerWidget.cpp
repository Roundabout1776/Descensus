#include "DesItemContainerWidget.h"

#include "SDesItemContainerWidget.h"
#include "SDesItemLayer.h"
#include "SDesItemWidget.h"
#include "Player/DesInventoryComponent.h"
#include "Components/DesItemContainerComponent.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "UI/DesStyle.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesItemContainerWidget::RebuildWidget()
{
	SAssignNew(Widget, SDesItemContainerWidget);
	return Widget.ToSharedRef();
}

void UDesItemContainerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (Widget.IsValid())
	{
#if WITH_EDITORONLY_DATA
		Widget->SetGridSize(PreviewGridSize);
#endif

		Widget->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
		Widget->SetOnMouseMove(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseMove));
		Widget->SetOnMouseLeave(BIND_UOBJECT_DELEGATE(FSimpleNoReplyPointerEventHandler, HandleMouseLeave));
	}
}

void UDesItemContainerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Widget.Reset();
}

void UDesItemContainerWidget::SetItemLayer(const TSharedRef<SDesItemLayer>& InItemLayer)
{
	ItemLayer = InItemLayer.ToSharedPtr();
}

void UDesItemContainerWidget::AttachToItemContainerComponent(UDesItemContainerComponent* InItemContainerComponent)
{
	check(InItemContainerComponent);

	DetachFromItemContainerComponent();

	ItemContainerComponent = MakeWeakObjectPtr(InItemContainerComponent);

	OnAnyChangesDelegateHandle = ItemContainerComponent->OnAnyChangesDelegate.AddUObject(
		this, &ThisClass::OnyAnyChanges);


	Widget->SetGridSize(ItemContainerComponent->GridSize);

	OnyAnyChanges(InItemContainerComponent->GetItemsRef());
}

void UDesItemContainerWidget::DetachFromItemContainerComponent()
{
	if (!ItemContainerComponent.IsValid())
		return;

	ItemContainerComponent->OnAnyChangesDelegate.Remove(OnAnyChangesDelegateHandle);
	ItemContainerComponent.Reset();

	Widget->CollapseAllItems();
}

void UDesItemContainerWidget::OnyAnyChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const
{
	Widget->CollapseAllItems();
	for (auto& Entry : ItemContainerEntries)
	{
		if (const auto ItemInstance = Entry.ItemInstance)
		{
			Widget->AddItem(Entry.Position, GetItemWidgetData(ItemInstance));
		}
	}
}

FReply UDesItemContainerWidget::HandleMouseButtonDown(const FGeometry& Geometry,
                                                      const FPointerEvent& PointerEvent) const
{
	const auto EjectedItem = ItemLayer->GetEjectedItem();

	FIntVector2 Coords;
	if (EjectedItem)
	{
		const auto Size = EjectedItem->GetItemData()->Size;
		Coords = GetCoordsUnderPointerForSize(Geometry, PointerEvent, Size);
	}
	else
	{
		Coords = GetCoordsUnderPointer(Geometry, PointerEvent);
	}

	if (const auto ItemInstance = ItemContainerComponent->GetItemInstance(Coords))
	{
		if (EjectedItem)
		{
			/* Swap item. */
			ItemLayer->GetInventoryComponent()->ServerMoveEjectedItem(ItemContainerComponent.Get(), Coords);
		}
		else
		{
			/* Eject item. */
			ItemLayer->GetInventoryComponent()->ServerEjectItem(ItemContainerComponent.Get(), ItemInstance);
			// ItemLayer->BeginItemMove(ItemContainerComponent.Get(), ItemInstance, GetItemWidgetData(ItemInstance),
			//                          PointerEvent.GetScreenSpacePosition());
		}
	}
	else
	{
		/* Move item. */
		if (EjectedItem && Widget->GetIsTelegraphVisible())
		{
			ItemLayer->GetInventoryComponent()->ServerMoveEjectedItem(ItemContainerComponent.Get(), Coords);
			// ItemLayer->EndItemMove();
		}
	}
	return FReply::Handled();
}

FReply UDesItemContainerWidget::HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const
{
	bool bIsTelegraphVisible = false;
	if (ItemLayer->GetEjectedItem())
	{
		const FIntVector2 ItemSize = ItemLayer->GetEjectedItem()->GetItemData()->Size;
		const auto Coords = GetCoordsUnderPointerForSize(Geometry, PointerEvent, ItemSize);
		const auto GridSize = Widget->GetGridSize();

		if (Coords.X + ItemSize.X - 1 >= GridSize.X || Coords.Y + ItemSize.Y - 1 >= GridSize.Y)
		{
		}
		else
		{
			bIsTelegraphVisible = true;
			const auto Style = FDesStyle::GetDefaultStyle();
			Widget->SetTelegraphSize(FVector2D(ItemSize.X * Style->CellSize, ItemSize.Y * Style->CellSize));
			Widget->SetTelegraphPosition(FVector2D{
				Coords.X * (Geometry.Size.X / GridSize.X), Coords.Y * (Geometry.Size.Y / GridSize.Y)
			});
		}
	}
	Widget->SetTelegraphVisible(bIsTelegraphVisible);
	return FReply::Unhandled();
}

void UDesItemContainerWidget::HandleMouseLeave(const FPointerEvent& PointerEvent) const
{
	Widget->SetTelegraphVisible(false);
}

FIntVector2 UDesItemContainerWidget::ClampCoords(const FIntVector2& InCoords) const
{
	const auto GridSize = Widget->GetGridSize();
	return FIntVector2(FMath::Clamp(InCoords.X, 0, GridSize.X - 1), FMath::Clamp(InCoords.Y, 0, GridSize.Y - 1));
}

FIntVector2 UDesItemContainerWidget::GetCoordsUnderPointerForSize(const FGeometry& Geometry,
                                                                  const FPointerEvent& PointerEvent,
                                                                  const FIntVector2& Size) const
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto GridSize = Widget->GetGridSize();
	FVector2D MouseLocal = Geometry.AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());
	MouseLocal -= FVector2D(Size.X * Style->CellSize / 2.0, Size.Y * Style->CellSize / 2.0);
	MouseLocal /= Geometry.Size;
	MouseLocal *= FVector2D(GridSize.X, GridSize.Y);
	return ClampCoords({FMath::RoundToInt32(MouseLocal.X), FMath::RoundToInt32(MouseLocal.Y)});
}

FIntVector2 UDesItemContainerWidget::GetCoordsUnderPointer(const FGeometry& Geometry,
                                                           const FPointerEvent& PointerEvent) const
{
	const auto GridSize = Widget->GetGridSize();
	FVector2D MouseLocal = Geometry.AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());
	MouseLocal /= Geometry.Size;
	MouseLocal *= FVector2D(GridSize.X, GridSize.Y);
	return ClampCoords({static_cast<int>(MouseLocal.X), static_cast<int>(MouseLocal.Y)});
}

FDesItemWidgetData UDesItemContainerWidget::GetItemWidgetData(const UDesItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		const auto ItemData = ItemInstance->GetItemData();

		return FDesItemWidgetData{
			ItemData->Size, ItemInstance->GetQuantity(),
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
