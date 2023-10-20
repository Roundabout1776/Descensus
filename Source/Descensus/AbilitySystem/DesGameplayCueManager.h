#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "DesGameplayCueManager.generated.h"

UCLASS()
class DESCENSUS_API UDesGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override
	{
		return false;
	}
};
