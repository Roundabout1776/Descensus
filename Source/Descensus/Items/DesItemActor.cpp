#include "DesItemActor.h"

#include "Items/DesItemData.h"
#include "Items/DesItemInstance.h"
#include "Net/UnrealNetwork.h"

ADesItemActor::ADesItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
}

void ADesItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemDataClass))
		{
			ItemInstance = NewObject<UDesItemInstance>();
			ItemInstance->Quantity = 1;
			ItemInstance->ItemDataClass = ItemDataClass;
		}
	}
}

void ADesItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADesItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADesItemActor, ItemInstance);
}
