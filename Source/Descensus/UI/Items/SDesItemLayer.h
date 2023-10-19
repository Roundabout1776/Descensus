#pragma once

#include "Widgets/SCanvas.h"

struct FItemContainerEntry;
struct FDesItemWidgetData;
class UDesItemContainerComponent;
class UDesItemInstance;
class UDesInventoryComponent;
class SDesItemWidget;
class SCanvas;

class DESCENSUS_API SDesItemLayer final : public SCompoundWidget
{
protected:
	TWeakObjectPtr<UDesInventoryComponent> InventoryComponent;
	// TArray<TSharedPtr<SDesItemWidget>> ItemWidgetPool;
	FVector2D CachedMousePosition;
	FVector2D EjectedItemOffset;
	TSharedPtr<SDesItemWidget> EjectedItemWidget;

public:
	SLATE_BEGIN_ARGS(SDesItemLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetEjectedItemPosition(const FVector2D& MousePosition);
	void UpdateEjectedItemQuantity(int32 Quantity, int32 MaxQuantity) const;
	void ShowEjectedItem(const FDesItemWidgetData& ItemWidgetData);
	void HideEjectedItem();
	const UDesItemInstance* GetEjectedItem() const;
	UDesInventoryComponent* GetInventoryComponent() const;
	void OnEjectedItemChanged(const UDesItemInstance* ItemInstance);
	void OnAnyChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const;
	void SetInventoryComponent(UDesInventoryComponent* InInventoryComponent);
};
