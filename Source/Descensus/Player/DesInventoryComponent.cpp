#include "DesInventoryComponent.h"

#include "Items/DesItemInstance.h"
#include "Items/DesItemData.h"
#include "DesGameState.h"
#include "DesLogging.h"
#include "DesPlayerCharacter.h"
#include "DesPlayerController.h"
#include "Net/UnrealNetwork.h"

UDesInventoryComponent::UDesInventoryComponent()
{
	GridSize = {10, 6, 0};
}

void UDesInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	CharacterOwner = MakeWeakObjectPtr(GetOwner<ADesPlayerCharacter>());
}

bool UDesInventoryComponent::CanInteractWithContainer(const UDesItemContainerComponent* Container)
{
	const auto PC = CharacterOwner->GetController<ADesPlayerController>();
	return PC->CheckIfCanInteractWithActor(Container->GetOwner());
}

void UDesInventoryComponent::ServerDestroyItem_Implementation(UDesItemInstance* InItemInstance)
{
	if (!InItemInstance)
	{
		return;
	}

	RemoveItemByInstance(InItemInstance);
	GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(InItemInstance);
}

void UDesInventoryComponent::ServerDestroyEjectedItem_Implementation()
{
	if (!EjectedItem)
	{
		return;
	}

	GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(EjectedItem);

	SetEjectedItem(nullptr);
}

void UDesInventoryComponent::ServerMoveEjectedItem_Implementation(UDesItemContainerComponent* Container,
                                                                  const FIntVector2 Coords)
{
	if (!Container)
	{
		return;
	}

	if (!CanInteractWithContainer(Container))
	{
		return;
	}

	if (!EjectedItem)
	{
		return;
	}

	const auto ItemData = EjectedItem->GetItemData();

	const auto InGridSize = Container->GridSize;

	/* Basic bound check. */
	if (Coords.X + ItemData->Size.X > InGridSize.X)
	{
		return;
	}
	if (Coords.Y + ItemData->Size.Y - 1 >= InGridSize.Y)
	{
		return;
	}

	/* Make sure the rect contains no more than 1 item. */
	int32 SoleItemGridValue = 0;
	for (auto X = Coords.X; X < Coords.X + ItemData->Size.X; X++)
	{
		for (auto Y = Coords.Y; Y < Coords.Y + ItemData->Size.Y; Y++)
		{
			if (const auto GridValue = Container->Grid[Y * InGridSize.X + X]; GridValue != 0)
			{
				if (GridValueToItemsIndex(GridValue) == GridValueToItemsIndex(SoleItemGridValue))
				{
					continue;
				}
				if (SoleItemGridValue != 0)
				{
					return;
				}
				SoleItemGridValue = GridValue;
			}
		}
	}

	/* Attempt to stack.
	 * @TODO: optimize. */
	if (SoleItemGridValue != 0)
	{
		const auto& EntryToStack = Container->GetItemsRef()[GridValueToItemsIndex(SoleItemGridValue)];

		if (EntryToStack.ItemInstance->GetItemData() == EjectedItem->GetItemData())
		{
			const auto MaxQuantity = EjectedItem->GetItemData()->MaxQuantity;
			if (EntryToStack.ItemInstance->GetQuantity() != MaxQuantity)
			{
				if (EntryToStack.ItemInstance->GetQuantity() + EjectedItem->GetQuantity() <= MaxQuantity)
				{
					/* Increase quantity of an existing item in the container
					 * and destroy ejected item. */
					EntryToStack.ItemInstance->SetQuantity(
						EntryToStack.ItemInstance->GetQuantity() + EjectedItem->GetQuantity());

					GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(EjectedItem);

					SetEjectedItem(nullptr);

					return;
				}
				/* Set quantity of an existing item in the container to max
					 * and subtract the difference from ejected item. */
				EjectedItem->SetQuantity(
					EjectedItem->GetQuantity() - (MaxQuantity - EntryToStack.ItemInstance->GetQuantity()));
				EntryToStack.ItemInstance->SetQuantity(MaxQuantity);

				return;
			}
		}
	}

	if (SoleItemGridValue != 0)
	{
		const auto EntryToSwap = Container->GetItemsRef()[GridValueToItemsIndex(SoleItemGridValue)];
		Container->RemoveItemByInstance(EntryToSwap.ItemInstance);

		Container->AddItem(EjectedItem, Coords);

		SetEjectedItem(EntryToSwap.ItemInstance);
	}
	else
	{
		Container->AddItem(EjectedItem, Coords);

		SetEjectedItem(nullptr);
	}
}

void UDesInventoryComponent::ServerEjectItem_Implementation(UDesItemContainerComponent* Container,
                                                            UDesItemInstance* InItemInstance)
{
	if (EjectedItem)
	{
		return;
	}

	if (!CanInteractWithContainer(Container))
	{
		return;
	}

	if (!Container->GetItemsRef().ContainsByPredicate([InItemInstance](const FItemContainerEntry& Entry)
	{
		return Entry.ItemInstance == InItemInstance;
	}))
	{
		return;
	}

	Container->RemoveItemByInstance(InItemInstance);

	SetEjectedItem(InItemInstance);
}

void UDesInventoryComponent::OnRep_EjectedItem()
{
	OnAnyChangesDelegate.Broadcast(GetItemsRef());
	OnEjectedItemChanged.Broadcast(EjectedItem);
}

void UDesInventoryComponent::SetEjectedItem(UDesItemInstance* ItemInstance)
{
	EjectedItem = ItemInstance;
	OnRep_EjectedItem();
}

void UDesInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS_FAST(UDesInventoryComponent, EjectedItem, Params);
}
