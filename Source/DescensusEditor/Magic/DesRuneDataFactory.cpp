#include "Magic/DesRuneDataFactory.h"

#include "Magic/DesRuneData.h"

UDesRuneDataFactory::UDesRuneDataFactory()
{
	SupportedClass = UDesRuneData::StaticClass();
	bCreateNew = true;
}

UObject* UDesRuneDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                               UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UDesRuneData>(InParent, InName, Flags, Context);
}
