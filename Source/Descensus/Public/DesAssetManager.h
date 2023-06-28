#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DesAssetManager.generated.h"

UCLASS()
class DESCENSUS_API UDesAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;

	static const FPrimaryAssetType RuneDataType;
};
