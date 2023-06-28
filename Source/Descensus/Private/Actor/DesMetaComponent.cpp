#include "Actor/DesMetaComponent.h"

#include "UI/DesTooltipData.h"

UDesMetaComponent::UDesMetaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Name = FText::FromString(TEXT("Default Name"));
}

void UDesMetaComponent::BeginPlay()
{
	Super::BeginPlay();
}

FDesTooltipData UDesMetaComponent::GetTooltipData() const
{
	return FDesTooltipData(Name);
}

void UDesMetaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
