#include "DesAssetManager.h"

#include "AbilitySystemGlobals.h"

const FPrimaryAssetType UDesAssetManager::ItemDataType("ItemData");
const FPrimaryAssetType UDesAssetManager::RuneDataType("RuneData");

void UDesAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
