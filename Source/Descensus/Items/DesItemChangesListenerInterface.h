#pragma once

#include "DesItemChangesListenerInterface.generated.h"

class UDesItemInstance;

UINTERFACE(MinimalAPI, Blueprintable)
class UDesItemChangesListenerInterface : public UInterface
{
	GENERATED_BODY()
};

class IDesItemChangesListenerInterface
{
	GENERATED_BODY()

public:
	virtual void OnQuantityChanged(const UDesItemInstance* ItemInstance, int32 OldQuantity, int32 NewQuantity) = 0;
};
