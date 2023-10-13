#pragma once

#include "CoreMinimal.h"
#include "DesItemContainerComponent.h"
#include "DesInventoryComponent.generated.h"

UCLASS(ClassGroup=(Descensus), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesInventoryComponent : public UDesItemContainerComponent
{
	GENERATED_BODY()
	
public:
	UDesInventoryComponent();
};
