#include "DesCharacterScreen.h"

#include "DesStyle.h"
#include "SDesItemContainerWidget.h"
#include "Components/DesInventoryComponent.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Player/DesPlayerCharacter.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesCharacterScreen::RebuildWidget()
{
	const auto Style = FDesStyle::GetDefaultStyle();

	const auto GridSize = InventoryComponent.IsValid() ? InventoryComponent->GridSize : FIntVector();

	SAssignNew(Root, SBorder)
		.BorderImage(&Style->CommonBox)
		.Padding(Style->Padding)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
				.WidthOverride(100.0f)
				.HeightOverride(100.0f)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
				.WidthOverride(GridSize.X * Style->CellSize)
				.HeightOverride(GridSize.Y * Style->CellSize)
			[
				SAssignNew(ItemContainerWidget, SDesItemContainerWidget)
				.GridSize(GridSize) /* @TODO: fix */
			]
		]
	];

	return Root.ToSharedRef();
}

void UDesCharacterScreen::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Root.Reset();
	ItemContainerWidget.Reset();
}

void UDesCharacterScreen::InitForCharacter(const ADesPlayerCharacter* Character)
{
	check(Character);

	InventoryComponent = MakeWeakObjectPtr(Character->Inventory);
	/* @TODO: Unbind! */
	InventoryComponent->OnAnyChangesDelegate.AddWeakLambda(this, [&](const TArray<FItemContainerEntry>& Items)
	{
		ItemContainerWidget->CollapseAllItems();
		for (auto& Entry : Items)
		{
			if (Entry.ItemInstance)
			{
				const auto ItemData = Entry.ItemInstance->GetItemData();
				ItemContainerWidget->AddItem(Entry.Position, Entry.ItemInstance->GetItemData()->Size,
				                             &ItemData->IconBrush);
			}
		}
	});

	/* Fill in initial items. */
	// for (const auto& Entry : InventoryComponent->GetItemsRef())
	// {
	// 	if (Entry.ItemInstance)
	// 	{
	// 		const auto ItemData = Entry.ItemInstance->GetItemData();
	// 		ItemContainerWidget->AddItem(Entry.Position, Entry.ItemInstance->GetItemData()->Size,
	// 		                             &ItemData->IconBrush);
	// 	}
	// }
}

#if WITH_EDITOR
const FText UDesCharacterScreen::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}
#endif

#undef LOCTEXT_NAMESPACE
