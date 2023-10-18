#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DesGameState.generated.h"

class UDesItemData;
class UDesItemInstance;

UCLASS()
class DESCENSUS_API ADesGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADesGameState();

	UFUNCTION(BlueprintCallable, Category="Descensus|Items")
	UDesItemInstance* CreateItemInstance(TSubclassOf<UDesItemData> ItemData, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category="Descensus|Items")
	void DestroyItemInstance(UDesItemInstance* ItemInstance);
};
