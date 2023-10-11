#include "DesItemInstance.h"

#include "DesItemData.h"
#include "Net/UnrealNetwork.h"

void UDesItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UDesItemInstance, ItemDataClass);
    DOREPLIFETIME(UDesItemInstance, ItemActor);
    DOREPLIFETIME(UDesItemInstance, Quantity);
}

TObjectPtr<UDesItemData> UDesItemInstance::GetItemData() const
{
    return ItemDataClass->GetDefaultObject<UDesItemData>();
}
