#include "DesItemContainerComponent.h"

#include "DesGameState.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<int32> CVarGridAlwaysDirty(
	TEXT("Des.GridAlwaysDirty"),
	0,
	TEXT("Forces container grids to be updated each frame."),
	ECVF_Cheat
);

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
	bReplicateUsingRegisteredSubObjectList = true;
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	Array.OwnerComponent = MakeWeakObjectPtr(this);
}

void UDesItemContainerComponent::InitializeComponent()
{
	/* @TODO: investigate CDO behavior. */
	Grid.SetNumZeroed(GridSize.X * GridSize.Y);
	Array.Items.Empty();
	Array.Items.Reserve(GridSize.X * GridSize.Y);
	Array.MarkArrayDirty();
}

void UDesItemContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarGridAlwaysDirty.GetValueOnAnyThread())
	{
		bGridDirty = true;
	}

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
	if (!IsValid(Entry.ItemInstance))
	{
		return;
	}
	bGridDirty = true;
	Entry.ItemInstance->ChangesListener = this;
	OnItemAddedDelegate.Broadcast(Entry);
}

void UDesItemContainerComponent::OnItemChanged(const FItemContainerEntry& Entry)
{
	if (!IsValid(Entry.ItemInstance))
	{
		return;
	}
	bGridDirty = true;
	Entry.ItemInstance->ChangesListener = this;
	OnItemChangedDelegate.Broadcast(Entry);
}

void UDesItemContainerComponent::OnItemRemoved(const FItemContainerEntry& Entry)
{
	if (!IsValid(Entry.ItemInstance))
	{
		return;
	}
	bGridDirty = true;
	Entry.ItemInstance->ChangesListener = nullptr;
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

void UDesItemContainerComponent::AddItem(UDesItemInstance* InItemInstance, const FIntVector2 Coords)
{
	auto& Items = Array.Items;
	FillGrid(Coords, InItemInstance->GetItemData()->Size, Items.Num() + 1);
	FItemContainerEntry& Entry = Items.AddDefaulted_GetRef();
	Entry.Position = Coords;
	Entry.ItemInstance = InItemInstance;
	Array.MarkItemDirty(Entry);
	OnItemAdded(Entry);
}

bool UDesItemContainerComponent::AddItemAuto(UDesItemInstance* InItemInstance)
{
	const auto ItemData = InItemInstance->GetItemData();

	/* Stack if possible. */
	for (auto ItemIter = Array.Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		if (auto& Entry = *ItemIter; Entry.ItemInstance && Entry.ItemInstance->GetItemData() == InItemInstance->
			GetItemData())
		{
			const auto MaxQuantity = ItemData->MaxQuantity;
			if (MaxQuantity > 1 && Entry.ItemInstance->GetQuantity() < MaxQuantity)
			{
				if (const auto Remainder = Entry.ItemInstance->GetQuantity() + InItemInstance->GetQuantity() -
					MaxQuantity; Remainder > 0)
				{
					Entry.ItemInstance->SetQuantity(MaxQuantity);
					Array.MarkItemDirty(Entry);
					OnItemChanged(Entry);

					InItemInstance->SetQuantity(Remainder);
					continue;
				}
				Entry.ItemInstance->SetQuantity(Entry.ItemInstance->GetQuantity() + InItemInstance->GetQuantity());
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
				AddItem(InItemInstance, {CurrentCellX, CurrentCellY});
				return true;
			}
		}
	}

	return false;
}

void UDesItemContainerComponent::RemoveItemByInstance(UDesItemInstance* InItemInstance)
{
	if (!InItemInstance)
	{
		return;
	}
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

void UDesItemContainerComponent::OnQuantityChanged(const UDesItemInstance* ItemInstance, int32 OldQuantity,
                                                   int32 NewQuantity)
{
	bGridDirty = true;
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
