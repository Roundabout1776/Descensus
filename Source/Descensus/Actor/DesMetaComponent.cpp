#include "Actor/DesMetaComponent.h"

#include "Net/UnrealNetwork.h"
#include "UI/DesTooltipData.h"

UDesMetaComponent::UDesMetaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Name = FText::FromString(TEXT("Default Name"));
	
	SetIsReplicated(true);
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

void UDesMetaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDesMetaComponent, Tags)
}
