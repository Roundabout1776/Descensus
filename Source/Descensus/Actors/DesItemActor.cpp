#include "DesItemActor.h"

#include "DesGameState.h"
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
			ItemInstance = GetWorld()->GetGameState<ADesGameState>()->CreateItemInstance(
				ItemDataClass, ItemDataClass->GetDefaultObject<UDesItemData>()->MaxQuantity);
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
