#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemGrid.generated.h"

class UDesInventoryComponent;
struct FInventoryGrid;
class SDesItemGrid;

UCLASS()
class DESCENSUS_API UDesItemGrid : public UWidget
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<SDesItemGrid> ItemGrid;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	TWeakObjectPtr<UDesInventoryComponent> Inventory;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	void AttachGrid(TObjectPtr<UDesInventoryComponent> Grid);
	void OnRepGrid();
};
