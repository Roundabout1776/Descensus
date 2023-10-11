#include "Magic/DesRuneData.h"

#include "DesAssetManager.h"

UDesRuneData::UDesRuneData(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (Templates.Num() < 1)
	{
		Templates.AddDefaulted();
	}
}

FPrimaryAssetType UDesRuneData::GetPrimaryAssetType() const
{
	return UDesAssetManager::RuneDataType;
}
