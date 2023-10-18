﻿#pragma once

#include "CoreMinimal.h"
#include "DesItemInstance.generated.h"

class IDesItemChangesListenerInterface;
class ADesItemActor;
class UDesItemData;

UCLASS(BlueprintType)
class DESCENSUS_API UDesItemInstance : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Quantity)
	int32 Quantity;

	UFUNCTION()
	void OnRep_Quantity(int32 OldQuantity) const;

public:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	TSubclassOf<UDesItemData> ItemDataClass;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TWeakObjectPtr<ADesItemActor> ItemActor;

	UPROPERTY(Transient)
	TScriptInterface<IDesItemChangesListenerInterface> ChangesListener;

	TObjectPtr<UDesItemData> GetItemData() const;

	virtual bool IsSupportedForNetworking() const override { return true; }

	void SetQuantity(int32 InQuantity);
	FORCEINLINE int32 GetQuantity() const { return Quantity; }
};
