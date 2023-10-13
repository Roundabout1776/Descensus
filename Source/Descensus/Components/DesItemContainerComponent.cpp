#include "DesItemContainerComponent.h"

#include "DesGameState.h"
#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Net/UnrealNetwork.h"

void FItemContainerEntry::PostReplicatedAdd(const FItemContainer& InArraySerializer)
{
	const auto _ = InArraySerializer.OnItemAddedDelegate.ExecuteIfBound(*this);
}

void FItemContainerEntry::PostReplicatedChange(const FItemContainer& InArraySerializer)
{
	const auto _ = InArraySerializer.OnItemChangedDelegate.ExecuteIfBound(*this);
}

void FItemContainerEntry::PreReplicatedRemove(const FItemContainer& InArraySerializer)
{
	const auto _ = InArraySerializer.OnItemRemovedDelegate.ExecuteIfBound(*this);
}

UDesItemContainerComponent::UDesItemContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UDesItemContainerComponent::InitializeComponent()
{
	/* @TODO: investigate */
	Grid.SetNumZeroed(GridSize.X * GridSize.Y);
}

void UDesItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		const auto GameState = GetWorld()->GetGameState<ADesGameState>();
		for (const auto ItemDataClass : DefaultItems)
		{
			AddItemAuto(GameState->CreateItemInstance(ItemDataClass));
		}
	}
}

void UDesItemContainerComponent::FillGrid(const FIntVector2 Coords, const FIntVector2 Size, UDesItemInstance* ItemInstance)
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
					if (const auto Cell = Grid[(Y * GridSize.X) + X])
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
				FillGrid(Entry.Position, ItemData->Size, Entry.ItemInstance);
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
		const FItemContainerEntry& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			Array.MarkArrayDirty();
			break;
		}
	}
}

void UDesItemContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDesItemContainerComponent, Array);
}
