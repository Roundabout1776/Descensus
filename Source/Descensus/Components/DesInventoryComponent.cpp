#include "DesInventoryComponent.h"

#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Net/UnrealNetwork.h"

void FInventoryGrid::AddItem(TSubclassOf<UDesItemData> InItemDataClass)
{
	FInventoryGridItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UDesItemInstance>();
	// Item.ItemInstance->Init(InItemStaticDataClass,
	//                         UActionGameStatics::GetItemStaticData(InItemStaticDataClass)->MaxStackCount);
	MarkItemDirty(Item);
}

void FInventoryGrid::AddItem(TObjectPtr<UDesItemInstance> InItemInstance)
{
	FInventoryGridItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

void FInventoryGrid::RemoveItem(TSubclassOf<UDesItemData> InItemDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		const FInventoryGridItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemData()->IsA(InItemDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

void FInventoryGrid::RemoveItem(TObjectPtr<UDesItemInstance> InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		const FInventoryGridItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

TArray<TObjectPtr<UDesItemInstance>> FInventoryGrid::GetAllInstancesWithTag(FGameplayTag Tag)
{
	TArray<TObjectPtr<UDesItemInstance>> OutInstances;
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryGridItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemData()->Tags.HasTag(Tag))
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}
	return OutInstances;
}

TArray<TObjectPtr<UDesItemInstance>> FInventoryGrid::GetAllAvailableInstancesOfType(
	TSubclassOf<UDesItemData> InItemDataClass)
{
	TArray<TObjectPtr<UDesItemInstance>> OutInstances;
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryGridItem& Item = *ItemIter;
		if (!Item.ItemInstance)
		{
			continue;
		}
		const UDesItemData* ItemData = Item.ItemInstance->GetItemData();
		const bool bSameType = ItemData->IsA(InItemDataClass);
		const bool bHasCapacity = ItemData->MaxStackCount > Item.ItemInstance->Quantity;
		if (bSameType && bHasCapacity)
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}
	return OutInstances;
}

UDesInventoryComponent::UDesInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UDesInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (const auto ItemDataClass : DefaultItems)
		{
			Grid.AddItem(ItemDataClass);
		}
	}
}

void UDesInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDesInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDesInventoryComponent::OnRep_Grid(const FInventoryGrid& OldGrid)
{
	const auto _ =OnRepGridDelegate.ExecuteIfBound();
}

void UDesInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDesInventoryComponent, Grid);
}
