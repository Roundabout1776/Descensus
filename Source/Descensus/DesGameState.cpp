#include "DesGameState.h"

#include "Items/DesItemInstance.h"

ADesGameState::ADesGameState()
{
	NetUpdateFrequency = 30.0;
	MinNetUpdateFrequency = 15.0;
	bReplicateUsingRegisteredSubObjectList = true;
}

UDesItemInstance* ADesGameState::CreateItemInstance(const TSubclassOf<UDesItemData> ItemData, const int32 Quantity)
{
	const auto ItemInstance = NewObject<UDesItemInstance>(this);
	ItemInstance->ItemDataClass = ItemData;
	ItemInstance->SetQuantity(Quantity);
	AddReplicatedSubObject(ItemInstance);
	return ItemInstance;
}

void ADesGameState::DestroyItemInstance(UDesItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}
