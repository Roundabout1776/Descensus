#include "DesItemContainerComponent.h"

#include "DesGameState.h"
#include "DesLogging.h"
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
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
	Array.OwnerComponent = this;
}

void UDesItemContainerComponent::InitializeComponent()
{
	/* @TODO: investigate CDO behavior */
	Grid.SetNumZeroed(GridSize.X * GridSize.Y);
}

void UDesItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		const auto GameState = GetWorld()->GetGameState<ADesGameState>();
		for (const auto& ItemDataClass : DefaultItems)
		{
			// AddItemAuto(GameState->CreateItemInstance(ItemDataClass));
		}
	}
}

void UDesItemContainerComponent::OnItemAdded(const FItemContainerEntry& Entry)
{
	if (!IsValid(Entry.ItemInstance))
	{
		DES_LOG_STR("OnItemAdded: Non-Valid ItemInstance!")
		return;
	}
	// checkf(IsValid(Entry.ItemInstance), TEXT("OnItemAdded: Non-Valid ItemInstance!"))
	
	FillGrid(Entry.Position, Entry.ItemInstance->GetItemData()->Size, Entry.ItemInstance);
	OnItemAddedDelegate.Broadcast(Entry);
	OnAnyChanges();
}

void UDesItemContainerComponent::OnItemChanged(const FItemContainerEntry& Entry)
{
	if (!IsValid(Entry.ItemInstance))
	{
		DES_LOG_STR("OnItemChanged: Non-Valid ItemInstance!")
		return;
	}
	// checkf(IsValid(Entry.ItemInstance), TEXT("OnItemChanged: Non-Valid ItemInstance!"))

	FillGrid(Entry.Position, Entry.ItemInstance->GetItemData()->Size, Entry.ItemInstance);
	OnItemChangedDelegate.Broadcast(Entry);
	OnAnyChanges();
}

void UDesItemContainerComponent::OnItemRemoved(const FItemContainerEntry& Entry)
{
	if (!IsValid(Entry.ItemInstance))
	{
		DES_LOG_STR("OnItemRemoved: Non-Valid ItemInstance!")
		return;
	}
	// checkf(IsValid(Entry.ItemInstance), TEXT("OnItemRemoved: Non-Valid ItemInstance!"))

	FillGrid(Entry.Position, Entry.ItemInstance->GetItemData()->Size, nullptr);
	OnItemRemovedDelegate.Broadcast(Entry);
	OnAnyChanges();
}

void UDesItemContainerComponent::OnAnyChanges()
{
	OnAnyChangesDelegate.Broadcast(GetItemsRef());
}

void UDesItemContainerComponent::FillGrid(const FIntVector2 Coords, const FIntVector2 Size,
                                          UDesItemInstance* ItemInstance)
{
	for (auto X = Coords.X; X < Coords.X + Size.X; X++)
	{
		for (auto Y = Coords.Y; Y < Coords.Y + Size.Y; Y++)
		{
			const auto Index = (GridSize.X * Y) + X;
			Grid[Y * GridSize.X + X] = ItemInstance;
		}
	}
}

bool UDesItemContainerComponent::AddItemAuto(UDesItemInstance* InItemInstance)
{
	const auto ItemData = InItemInstance->GetItemData();

	/* @TODO: optimize */
	for (auto GridIndex = 0; GridIndex < Grid.Num(); GridIndex++)
	{
		const auto CurrentCell = Grid[GridIndex];
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
		if (!CurrentCell)
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
					if (const auto Cell = Grid[Y * GridSize.X + X])
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

void UDesItemContainerComponent::RemoveItem(UDesItemInstance* InItemInstance)
{
	for (auto ItemIter = GetItemsRef().CreateIterator(); ItemIter; ++ItemIter)
	{
		if (const FItemContainerEntry& Entry = *ItemIter; Entry.ItemInstance && Entry.ItemInstance == InItemInstance)
		{
			FillGrid(Entry.Position, Entry.ItemInstance->GetItemData()->Size, nullptr);
			ItemIter.RemoveCurrent();
			Array.MarkArrayDirty();
			OnItemRemoved(Entry);
			break;
		}
	}
}

void UDesItemContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDesItemContainerComponent, Array);
}
