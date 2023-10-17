#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Items/DesItemInstance.h"
#include "DesItemLayer.generated.h"

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
	TSharedPtr<SDesItemLayer> Widget;
	bool bIsLocked;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	void ShowEjectedItem(const UDesItemInstance* InItem);
	void HideEjectedItem();

public:
	UPROPERTY(Transient)
	TWeakObjectPtr<UDesInventoryComponent> InventoryComponent;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void Lock() { bIsLocked = true; }
	const UDesItemInstance* GetEjectedItem() const;
	bool IsLocked() const;
	void UpdateEjectedItemPosition(const FVector2D& MousePosition, float DeltaTime) const;
	void OnEjectedItemChanged(const UDesItemInstance* DesItemInstance);
	void AttachToInventory(UDesInventoryComponent* InInventoryComponent);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
