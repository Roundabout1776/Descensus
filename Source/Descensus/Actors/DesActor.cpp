#include "DesActor.h"

#include "Components/DesMetaComponent.h"

ADesActor::ADesActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MetaComponent = CreateDefaultSubobject<UDesMetaComponent>(TEXT("DescensusMeta"));
}

void ADesActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADesActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
