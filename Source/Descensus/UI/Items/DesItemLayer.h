#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Items/DesItemInstance.h"
#include "DesItemLayer.generated.h"

struct FItemContainerEntry;
class UDesInventoryComponent;
class ADesPlayerState;
class UDesItemInstance;
struct FDesItemWidgetData;
class UDesItemContainerComponent;
class SDesItemLayer;
class SDesItemWidget;

UCLASS()
class DESCENSUS_API UDesItemLayer : public UWidget
{
	GENERATED_BODY()

protected:
	// TArray<TSharedPtr<SDesItemWidget>> ItemWidgetPool;
	FDelegateHandle OnItemEjectedChangedHandle;
	FDelegateHandle OnAnyChangesHandle;
	TSharedPtr<SDesItemLayer> Widget;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	void ShowEjectedItem(const UDesItemInstance* InItem);
	void HideEjectedItem();

public:
	UPROPERTY(Transient)
	TWeakObjectPtr<UDesInventoryComponent> InventoryComponent;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	const UDesItemInstance* GetEjectedItem() const;
	void UpdateEjectedItemPosition(const FVector2D& MousePosition, float DeltaTime) const;
	void OnEjectedItemChanged(const UDesItemInstance* ItemInstance);
	void OnAnyChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const;
	void AttachToInventory(UDesInventoryComponent* InInventoryComponent);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
