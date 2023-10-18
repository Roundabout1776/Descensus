#include "DesItemInstance.h"

#include "DesItemChangesListenerInterface.h"
#include "DesItemData.h"
#include "Net/UnrealNetwork.h"

void UDesItemInstance::OnRep_Quantity(const int32 OldQuantity) const
{
    if (ChangesListener)
    {
        ChangesListener->OnQuantityChanged(this, OldQuantity, Quantity);
    }
}

TObjectPtr<UDesItemData> UDesItemInstance::GetItemData() const
{
    return ItemDataClass->GetDefaultObject<UDesItemData>();
}

void UDesItemInstance::SetQuantity(const int32 InQuantity)
{
    const auto OldQuantity = Quantity;
    Quantity = InQuantity;
    OnRep_Quantity(OldQuantity);
}

void UDesItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(UDesItemInstance, ItemDataClass, COND_InitialOnly);
    DOREPLIFETIME(UDesItemInstance, ItemActor);
    DOREPLIFETIME_CONDITION_NOTIFY(UDesItemInstance, Quantity, COND_None, REPNOTIFY_OnChanged);
}
