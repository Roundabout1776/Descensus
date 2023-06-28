#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DesRuneDataFactory.generated.h"

UCLASS()
class DESCENSUSEDITOR_API UDesRuneDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDesRuneDataFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};
