#include "DesCharacterScreen.h"

#include "DesLogging.h"
#include "DesStyle.h"
#include "SDesItemContainerWidget.h"
#include "Components/DesInventoryComponent.h"
#include "Player/DesPlayerCharacter.h"

#define LOCTEXT_NAMESPACE "Descensus"

TSharedRef<SWidget> UDesCharacterScreen::RebuildWidget()
{
	const auto Style = FDesStyle::GetDefaultStyle();
	
	SAssignNew(Root, SBorder)
		.BorderImage(&Style->CommonBox)
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
				SAssignNew(Inventory, SDesItemContainerWidget)
				.GridSize({11, 7})
			]
		];

	return Root.ToSharedRef();
}

void UDesCharacterScreen::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Root.Reset();
	Inventory.Reset();
}

void UDesCharacterScreen::InitForCharacter(const ADesPlayerCharacter* Character)
{
	check(Character);

	InventoryComponent = MakeWeakObjectPtr(Character->Inventory);
	InventoryComponent->OnRepGridDelegate.BindUObject(this, &ThisClass::OnRepGrid);
	for (auto Item : InventoryComponent->Grid.GetItemsRef())
	{
		DES_LOG_BOOL("Item On Client", true)
	}
}

void UDesCharacterScreen::OnRepGrid()
{
	for (auto Item : InventoryComponent->Grid.GetItemsRef())
	{
		DES_LOG_BOOL("Item On Client OnRepGrid", true)
	}
}

#if WITH_EDITOR
const FText UDesCharacterScreen::GetPaletteCategory()
{
	return LOCTEXT("Descensus", "Descensus");
}
#endif

#undef LOCTEXT_NAMESPACE
