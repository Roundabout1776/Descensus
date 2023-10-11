#include "DesItemGrid.h"

#include "SDesItemGrid.h"

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

#undef LOCTEXT_NAMESPACE
