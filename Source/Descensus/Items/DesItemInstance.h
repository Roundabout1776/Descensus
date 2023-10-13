#pragma once

#include "CoreMinimal.h"
#include "DesItemInstance.generated.h"

class ADesItemActor;
class UDesItemData;

UCLASS(BlueprintType)
class DESCENSUS_API UDesItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	TSubclassOf<UDesItemData> ItemDataClass;

	UPROPERTY(Replicated)
	int32 Quantity;

	UPROPERTY(Replicated)
	TWeakObjectPtr<ADesItemActor> ItemActor;

	TObjectPtr<UDesItemData> GetItemData() const;

	virtual bool IsSupportedForNetworking() const override { return true; }
};
