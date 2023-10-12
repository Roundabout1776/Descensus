#pragma once

#include "CoreMinimal.h"
#include "Actors/DesActor.h"
#include "DesItemActor.generated.h"

class UDesItemData;
class UDesItemInstance;

UCLASS()
class DESCENSUS_API ADesItemActor : public ADesActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated)
	TObjectPtr<UDesItemInstance> ItemInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Item")
	TSubclassOf<UDesItemData> ItemDataClass;
	
	ADesItemActor();
	virtual void Tick(float DeltaTime) override;
};
