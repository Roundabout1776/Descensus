#include "DesInventoryComponent.h"

#include "Items/DesItemInstance.h"
#include "Items/DesItemData.h"
#include "DesGameState.h"
#include "DesLogging.h"
#include "Net/UnrealNetwork.h"

UDesInventoryComponent::UDesInventoryComponent()
{
	GridSize = {10, 6, 0};
}

void UDesInventoryComponent::ServerDestroyItem_Implementation(UDesItemInstance* InItemInstance)
{
	if (!InItemInstance)
		return;

	RemoveItemByInstance(InItemInstance);
	GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(InItemInstance);
}

void UDesInventoryComponent::ServerDestroyEjectedItem_Implementation()
{
	if (!EjectedItem)
		return;

	GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(EjectedItem);

	EjectedItem = nullptr;
	OnRep_EjectedItem();
}

void UDesInventoryComponent::ServerMoveEjectedItem_Implementation(UDesItemContainerComponent* Container,
                                                                  const FIntVector2 Coords)
{
	FItemContainerEntry& Entry = Container->GetItemsRef().AddDefaulted_GetRef();
	Entry.Position = Coords;
	Entry.ItemInstance = EjectedItem;
	Container->Array.MarkItemDirty(Entry);
	Container->OnItemAdded(Entry);
	
	EjectedItem = nullptr;
	OnRep_EjectedItem();
}

bool UDesInventoryComponent::ServerMoveEjectedItem_Validate(UDesItemContainerComponent* Container,
                                                            const FIntVector2 Coords)
{
	if (!Container)
		return false;

	if (!CanInteractWithContainer(Container))
		return false;

	if (!EjectedItem)
		return false;

	const auto ItemData = EjectedItem->GetItemData();

	const auto InGridSize = Container->GridSize;

	if (Coords.X + ItemData->Size.X > InGridSize.X)
	{
		return false;
	}
	if (Coords.Y + ItemData->Size.Y - 1 >= InGridSize.Y)
	{
		return false;
	}

	for (auto X = Coords.X; X < Coords.X + ItemData->Size.X; X++)
	{
		for (auto Y = Coords.Y; Y < Coords.Y + ItemData->Size.Y; Y++)
		{
			if (const auto GridValue = Container->Grid[Y * InGridSize.X + X]; GridValue != 0)
			{
				return false;
			}
		}
	}
	return true;
}

void UDesInventoryComponent::ServerEjectItem_Implementation(UDesItemContainerComponent* Container,
                                                            UDesItemInstance* InItemInstance)
{
	if (!CanInteractWithContainer(Container))
		return;

	if (!Container->GetItemsRef().ContainsByPredicate([InItemInstance](const FItemContainerEntry& Entry)
	{
		return Entry.ItemInstance == InItemInstance;
	}))
		return;

	/* @TODO: swap here? */
	if (EjectedItem)
		return;

	Container->RemoveItemByInstance(InItemInstance);
	
	EjectedItem = InItemInstance;
	OnRep_EjectedItem();
}

void UDesInventoryComponent::OnRep_EjectedItem() const
{
	OnEjectedItemChanged.Broadcast(EjectedItem);
}

void UDesInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDesInventoryComponent, EjectedItem, COND_OwnerOnly, REPNOTIFY_OnChanged);
}
