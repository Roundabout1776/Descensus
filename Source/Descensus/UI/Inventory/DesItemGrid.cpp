#include "DesItemGrid.h"

#include "DesLogging.h"
#include "SDesItemGrid.h"
#include "Components/DesInventoryComponent.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesItemGrid::RebuildWidget()
{
	SAssignNew(ItemGrid, SDesItemGrid).GridSize({7, 11});

	return ItemGrid.ToSharedRef();
}

void UDesItemGrid::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	ItemGrid.Reset();
}

#if WITH_EDITOR
const FText UDesItemGrid::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}
#endif

void UDesItemGrid::AttachGrid(const TObjectPtr<UDesInventoryComponent> Grid)
{
	Inventory = MakeWeakObjectPtr(Grid);
	Inventory->OnRepGridDelegate.BindUObject(this, &ThisClass::OnRepGrid);
	for (auto Item : Inventory->Grid.GetItemsRef())
	{
		DES_LOG_BOOL("Item On Client", true)
	}
}

void UDesItemGrid::OnRepGrid()
{
	for (auto Item : Inventory->Grid.GetItemsRef())
	{
		DES_LOG_BOOL("Item On Client Rep", true)
	}
}

#undef LOCTEXT_NAMESPACE
