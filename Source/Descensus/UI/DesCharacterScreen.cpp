#include "DesCharacterScreen.h"

#include "DesLogging.h"
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
			SAssignNew(ItemContainerWidget, SDesItemContainerWidget)
			.GridSize(InventoryComponent.IsValid() ? InventoryComponent->GridSize : FIntVector()) /* @TODO: fix */
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
	InventoryComponent->GetOnItemAddedDelegate().BindWeakLambda(this, [this](const FItemContainerEntry& Entry)
	{
		if (Entry.ItemInstance)
		{
			const auto ItemData = Entry.ItemInstance->GetItemData();
			ItemContainerWidget->AddItem(Entry.Position, Entry.ItemInstance->GetItemData()->Size,
			                             &ItemData->IconBrush);
			// DES_LOG_CSTR("NewItemClient!!", *Entry.ItemInstance->GetName())
		}
	});
}

#if WITH_EDITOR
const FText UDesCharacterScreen::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}
#endif

#undef LOCTEXT_NAMESPACE
