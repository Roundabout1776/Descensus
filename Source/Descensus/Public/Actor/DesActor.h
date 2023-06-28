#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DesActor.generated.h"

class UDesMetaComponent;

UCLASS()
class DESCENSUS_API ADesActor : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ADesActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Actor")
	TObjectPtr<UDesMetaComponent> MetaComponent;
};
