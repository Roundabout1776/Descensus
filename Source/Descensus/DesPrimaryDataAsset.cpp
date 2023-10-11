#include "DesPrimaryDataAsset.h"

FPrimaryAssetId UDesPrimaryDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GetPrimaryAssetType(), UniqueId);
}

FPrimaryAssetType UDesPrimaryDataAsset::GetPrimaryAssetType() const
{
	return FPrimaryAssetType("Invalid");
}
