#include "DesItemContainerComponent.h"

#include "DesGameState.h"
#include "DesLogging.h"
#include "IDetailTreeNode.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Net/UnrealNetwork.h"

void FItemContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const auto Index : AddedIndices)
	{
		OwnerComponent->OnItemAdded(Items[Index]);
	}
}

void FItemContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const auto Index : ChangedIndices)
	{
		OwnerComponent->OnItemChanged(Items[Index]);
	}
}

void FItemContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const auto Index : RemovedIndices)
	{
		OwnerComponent->OnItemRemoved(Items[Index]);
	}
}

UDesItemContainerComponent::UDesItemContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
	Array.OwnerComponent = this;
}

void UDesItemContainerComponent::InitializeComponent()
{
	/* @TODO: investigate CDO behavior */
	Grid.SetNumZeroed(GridSize.X * GridSize.Y);
	Array.Items.Reserve(GridSize.X * GridSize.Y);
	Array.MarkArrayDirty();
}

void UDesItemContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bGridDirty)
	{
		OnAnyChanges();
		bGridDirty = false;
	}
}

void UDesItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		const auto GameState = GetWorld()->GetGameState<ADesGameState>();
		for (const auto& ItemDataClass : DefaultItems)
		{
			AddItemAuto(GameState->CreateItemInstance(ItemDataClass));
		}
	}
}

void UDesItemContainerComponent::OnItemAdded(const FItemContainerEntry& Entry)
{
	bGridDirty = true;
	if (!IsValid(Entry.ItemInstance))
	{
		return;
	}
	OnItemAddedDelegate.Broadcast(Entry);
}

void UDesItemContainerComponent::OnItemChanged(const FItemContainerEntry& Entry)
{
	bGridDirty = true;
	if (!IsValid(Entry.ItemInstance))
	{
		return;
	}
	OnItemChangedDelegate.Broadcast(Entry);
}

void UDesItemContainerComponent::OnItemRemoved(const FItemContainerEntry& Entry)
{
	bGridDirty = true;
	if (!IsValid(Entry.ItemInstance))
	{
		return;
	}
	OnItemRemovedDelegate.Broadcast(Entry);
}

void UDesItemContainerComponent::OnAnyChanges()
{
	RebuildGrid();
	OnAnyChangesDelegate.Broadcast(GetItemsRef());
}

void UDesItemContainerComponent::FillGrid(const FIntVector2 Coords, const FIntVector2 Size,
                                          const int32 InIndex)
{
	for (auto X = Coords.X; X < Coords.X + Size.X; X++)
	{
		for (auto Y = Coords.Y; Y < Coords.Y + Size.Y; Y++)
		{
			Grid[Y * GridSize.X + X] = -InIndex;
		}
	}
	Grid[Coords.Y * GridSize.X + Coords.X] = InIndex;
}

void UDesItemContainerComponent::RebuildGrid()
{
	for (auto& GridValue : Grid)
	{
		GridValue = 0;
	}
	for (int Index = 0; Index < Array.Items.Num(); ++Index)
	{
		if (const auto& Entry = Array.Items[Index]; Entry.ItemInstance)
		{
			FillGrid(Entry.Position, Entry.ItemInstance->GetItemData()->Size, Index + 1);
		}
	}
}

bool UDesItemContainerComponent::AddItemAuto(UDesItemInstance* InItemInstance)
{
	const auto ItemData = InItemInstance->GetItemData();

	/* Stack if possible. */
	for (auto ItemIter = Array.Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		if (auto& Entry = *ItemIter; Entry.ItemInstance->GetItemData() == InItemInstance->GetItemData())
		{
			if (ItemData->MaxQuantity > 1 && Entry.ItemInstance->Quantity < ItemData->MaxQuantity)
			{
				if (const auto Remainder = Entry.ItemInstance->Quantity + InItemInstance->Quantity - ItemData->
					MaxQuantity; Remainder > 0)
				{
					Entry.ItemInstance->Quantity = ItemData->MaxQuantity;
					Array.MarkItemDirty(Entry);
					OnItemChanged(Entry);

					InItemInstance->Quantity = Remainder;
					continue;
				}
				Entry.ItemInstance->Quantity += InItemInstance->Quantity;
				Array.MarkItemDirty(Entry);
				OnItemChanged(Entry);
				GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(InItemInstance);
				return true;
			}
		}
	}
	for (auto GridIndex = 0; GridIndex < Grid.Num(); GridIndex++)
	{
		const auto CurrentCellY = GridIndex / GridSize.X;
		const auto CurrentCellX = GridIndex - (CurrentCellY * GridSize.X);
		if (CurrentCellX + ItemData->Size.X > GridSize.X)
		{
			GridIndex += GridSize.X - CurrentCellX - 1;
			continue;
		}
		if (CurrentCellY + ItemData->Size.Y - 1 >= GridSize.Y)
		{
			return false;
		}
		if (Grid[GridIndex] == 0)
		{
			bool bFree = true;
			for (auto X = CurrentCellX; X < CurrentCellX + ItemData->Size.X; X++)
			{
				if (!bFree)
				{
					break;
				}
				for (auto Y = CurrentCellY; Y < CurrentCellY + ItemData->Size.Y; Y++)
				{
					if (Grid[Y * GridSize.X + X] != 0)
					{
						bFree = false;
						break;
					}
				}
			}
			if (bFree)
			{
				auto& Items = Array.Items;
				FItemContainerEntry& Entry = Items.AddDefaulted_GetRef();
				Entry.Position = {CurrentCellX, CurrentCellY};
				Entry.ItemInstance = InItemInstance;
				Array.MarkItemDirty(Entry);
				OnItemAdded(Entry);
				return true;
			}
		}
	}

	return false;
}

void UDesItemContainerComponent::RemoveItemByInstance(UDesItemInstance* InItemInstance)
{
	for (auto ItemIter = GetItemsRef().CreateIterator(); ItemIter; ++ItemIter)
	{
		if (const FItemContainerEntry& Entry = *ItemIter; Entry.ItemInstance && Entry.ItemInstance == InItemInstance)
		{
			FillGrid(Entry.Position, Entry.ItemInstance->GetItemData()->Size, 0);
			ItemIter.RemoveCurrent();
			Array.MarkArrayDirty();
			OnItemRemoved(Entry);
			break;
		}
	}
}

UDesItemInstance* UDesItemContainerComponent::GetItemInstance(const FIntVector2 Coords)
{
	if (const auto ItemIndex = GridCoordsToItemsIndex(Coords); Array.Items.IsValidIndex(ItemIndex))
	{
		return Array.Items[ItemIndex].ItemInstance;
	}
	return nullptr;
}

void UDesItemContainerComponent::ServerDestroyItem_Implementation(UDesItemInstance* InItemInstance)
{
	if (!InItemInstance)
		return;

	RemoveItemByInstance(InItemInstance);
	GetWorld()->GetGameState<ADesGameState>()->DestroyItemInstance(InItemInstance);
}

void UDesItemContainerComponent::ServerMoveItem_Implementation(UDesItemInstance* InItemInstance,
                                                               const FIntVector2 Coords)
{
	const auto Entry = Array.Items.FindByPredicate([InItemInstance](const FItemContainerEntry& Entry)
	{
		return Entry.ItemInstance == InItemInstance;
	});

	Entry->Position = Coords;
	Array.MarkItemDirty(*Entry);
	OnItemChanged(*Entry);
}

bool UDesItemContainerComponent::ServerMoveItem_Validate(UDesItemInstance* InItemInstance, const FIntVector2 Coords)
{
	if (!InItemInstance)
		return false;

	if (Array.Items.ContainsByPredicate([InItemInstance](const FItemContainerEntry& CurrentEntry)
	{
		return CurrentEntry.ItemInstance == InItemInstance;
	}))
	{
		const auto ItemData = InItemInstance->GetItemData();

		if (Coords.X + ItemData->Size.X > GridSize.X)
		{
			return false;
		}
		if (Coords.Y + ItemData->Size.Y - 1 >= GridSize.Y)
		{
			return false;
		}

		for (auto X = Coords.X; X < Coords.X + ItemData->Size.X; X++)
		{
			for (auto Y = Coords.Y; Y < Coords.Y + ItemData->Size.Y; Y++)
			{
				const auto GridValue = Grid[Y * GridSize.X + X];
				if (GridValue == 0)
				{
					continue;
				}
				if (const auto CurrentEntry = Array.Items[GridValueToItemsIndex(GridValue)]; CurrentEntry.ItemInstance
					!= InItemInstance)
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

int32 UDesItemContainerComponent::IntVectorToIndex(const FIntVector2 Coords) const
{
	return Coords.Y * GridSize.X + Coords.X;
}

int32 UDesItemContainerComponent::GridValueToItemsIndex(const int32 Value)
{
	return FMath::Abs(Value) - 1;
}

int32 UDesItemContainerComponent::GridCoordsToItemsIndex(FIntVector2 Coords) const
{
	return GridValueToItemsIndex(Grid[IntVectorToIndex(Coords)]);
}

void UDesItemContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDesItemContainerComponent, Array);
}
