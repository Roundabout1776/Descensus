#include "DesGameState.h"

#include "Items/DesItemInstance.h"

ADesGameState::ADesGameState()
{
	bReplicateUsingRegisteredSubObjectList = true;
}

UDesItemInstance* ADesGameState::CreateItemInstance(const TSubclassOf<UDesItemData> ItemData)
{
	const auto ItemInstance = NewObject<UDesItemInstance>(this);
	ItemInstance->ItemDataClass = ItemData;
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
