#include "DesItemContainerWidget.h"

#include "DesLogging.h"
#include "SDesItemContainerWidget.h"
#include "Components/DesInventoryComponent.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesItemContainerWidget::RebuildWidget()
{
	SAssignNew(ItemContainerWidget, SDesItemContainerWidget).GridSize({7, 11});

	return ItemContainerWidget.ToSharedRef();
}

void UDesItemContainerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	ItemContainerWidget.Reset();
}

#if WITH_EDITOR
const FText UDesItemContainerWidget::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}
#endif

void UDesItemContainerWidget::AttachContainer(const TObjectPtr<UDesItemContainerComponent> InItemContainerComponent)
{
	ItemContainerComponent = MakeWeakObjectPtr(InItemContainerComponent);
	ItemContainerComponent->OnRepGridDelegate.BindUObject(this, &ThisClass::OnRepGrid);
	for (auto Item : ItemContainerComponent->Grid.GetItemsRef())
	{
		DES_LOG_BOOL("Item On Client", true)
	}
}

void UDesItemContainerWidget::OnRepGrid()
{
	for (auto Item : ItemContainerComponent->Grid.GetItemsRef())
	{
		DES_LOG_BOOL("Item On Client Rep", true)
	}
}

#undef LOCTEXT_NAMESPACE
