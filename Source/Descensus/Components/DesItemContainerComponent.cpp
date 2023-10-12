#include "DesItemContainerComponent.h"

#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Net/UnrealNetwork.h"

void FItemContainer::AddItem(TSubclassOf<UDesItemData> InItemDataClass)
{
	FItemContainerEntry& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UDesItemInstance>();
	// Item.ItemInstance->Init(InItemStaticDataClass,
	//                         UActionGameStatics::GetItemStaticData(InItemStaticDataClass)->MaxStackCount);
	MarkItemDirty(Item);
}

void FItemContainer::AddItem(TObjectPtr<UDesItemInstance> InItemInstance)
{
	FItemContainerEntry& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

void FItemContainer::RemoveItem(TSubclassOf<UDesItemData> InItemDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		const FItemContainerEntry& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemData()->IsA(InItemDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

void FItemContainer::RemoveItem(TObjectPtr<UDesItemInstance> InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		const FItemContainerEntry& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

TArray<TObjectPtr<UDesItemInstance>> FItemContainer::GetAllInstancesWithTag(FGameplayTag Tag)
{
	TArray<TObjectPtr<UDesItemInstance>> OutInstances;
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FItemContainerEntry& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemData()->Tags.HasTag(Tag))
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}
	return OutInstances;
}

TArray<TObjectPtr<UDesItemInstance>> FItemContainer::GetAllAvailableInstancesOfType(
	TSubclassOf<UDesItemData> InItemDataClass)
{
	TArray<TObjectPtr<UDesItemInstance>> OutInstances;
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FItemContainerEntry& Item = *ItemIter;
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

UDesItemContainerComponent::UDesItemContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UDesItemContainerComponent::InitializeComponent()
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

void UDesItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDesItemContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDesItemContainerComponent::OnRep_Grid(const FItemContainer& OldGrid)
{
	const auto _ = OnRepGridDelegate.ExecuteIfBound();
}

void UDesItemContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDesItemContainerComponent, Grid);
}
