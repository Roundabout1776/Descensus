#pragma once

#include "CoreMinimal.h"
#include "DesActor.h"
#include "DesContainerActor.generated.h"

class UDesItemContainerComponent;

UCLASS(Abstract)
class DESCENSUS_API ADesContainerActor : public ADesActor
{
	GENERATED_BODY()

protected:
	bool bIsOpened;

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Descensus|Actor")
	TObjectPtr<UDesItemContainerComponent> ItemContainer;

	ADesContainerActor();
	virtual void Tick(float DeltaTime) override;
	virtual ELifetimeCondition
	AllowActorComponentToReplicate(const UActorComponent* ComponentToReplicate) const override;

	void SetOpened();
};
