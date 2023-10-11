#include "DesItemData.h"

#include "DesAssetManager.h"

FPrimaryAssetType UDesItemData::GetPrimaryAssetType() const
{
	return UDesAssetManager::ItemDataType;
}
