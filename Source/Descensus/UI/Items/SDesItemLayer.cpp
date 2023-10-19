#include "SDesItemLayer.h"

#include "DesItemContainerWidget.h"
#include "SDesItemWidget.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Player/DesInventoryComponent.h"
#include "UI/DesStyle.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemLayer::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::Hidden);

	ChildSlot
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		[
			SAssignNew(EjectedItemWidget, SDesItemWidget)
		];
}

void SDesItemLayer::SetEjectedItemPosition(const FVector2D& MousePosition)
{
	CachedMousePosition = MousePosition;

	if (GetVisibility() == EVisibility::HitTestInvisible)
	{
		const auto TargetPosition = CachedMousePosition - EjectedItemOffset;

		FSlateRenderTransform Transform;
		Transform.SetTranslation(TargetPosition);
		SetRenderTransform(Transform);
	}
}

void SDesItemLayer::UpdateEjectedItemQuantity(const int32 Quantity, const int32 MaxQuantity) const
{
	EjectedItemWidget->SetQuantity(Quantity, MaxQuantity);
}

void SDesItemLayer::ShowEjectedItem(const FDesItemWidgetData& ItemWidgetData)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto Size = FVector2D(ItemWidgetData.Size.X * Style->CellSize,
	                            ItemWidgetData.Size.Y * Style->CellSize);
	
	EjectedItemOffset = Size / 2.0;

	FSlateRenderTransform Transform;
	Transform.SetTranslation(CachedMousePosition - EjectedItemOffset);
	SetRenderTransform(Transform);

	EjectedItemWidget->SetData(ItemWidgetData);
	SetVisibility(EVisibility::HitTestInvisible);
}

void SDesItemLayer::HideEjectedItem()
{
	SetVisibility(EVisibility::Hidden);
}

const UDesItemInstance* SDesItemLayer::GetEjectedItem() const
{
	return InventoryComponent->GetEjectedItem();
}

UDesInventoryComponent* SDesItemLayer::GetInventoryComponent() const
{
	return InventoryComponent.Get();
}

void SDesItemLayer::OnEjectedItemChanged(const UDesItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ShowEjectedItem(UDesItemContainerWidget::GetItemWidgetData(ItemInstance));
	}
	else
	{
		HideEjectedItem();
	}
}

void SDesItemLayer::OnAnyChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const
{
	if (const auto EjectedItem = GetEjectedItem())
	{
		UpdateEjectedItemQuantity(EjectedItem->GetQuantity(), EjectedItem->GetItemData()->MaxQuantity);
	}
}

void SDesItemLayer::SetInventoryComponent(UDesInventoryComponent* InInventoryComponent)
{
	InventoryComponent = MakeWeakObjectPtr(InInventoryComponent);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
