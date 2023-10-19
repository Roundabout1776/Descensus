#include "DesContainerActor.h"

#include "Components/DesItemContainerComponent.h"

ADesContainerActor::ADesContainerActor()
{
	NetUpdateFrequency = 30.0f;
	MinNetUpdateFrequency = 15.0f;
	bReplicates = true;
	
	ItemContainer = CreateDefaultSubobject<UDesItemContainerComponent>(TEXT("ItemContainer"));
	AddReplicatedSubObject(ItemContainer, COND_Never);
}

void ADesContainerActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADesContainerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ELifetimeCondition ADesContainerActor::AllowActorComponentToReplicate(const UActorComponent* ComponentToReplicate) const
{
	if (!bIsOpened)
	{
		if (ComponentToReplicate->IsA<UDesItemContainerComponent>())
		{
			return COND_Never;
		}
	}
	return Super::AllowActorComponentToReplicate(ComponentToReplicate);
}

void ADesContainerActor::SetOpened()
{
	bIsOpened = true;
	SetReplicatedComponentNetCondition(ItemContainer, COND_None);
}
