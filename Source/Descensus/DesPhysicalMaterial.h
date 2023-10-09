#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DesPhysicalMaterial.generated.h"

class UNiagaraSystem;

UCLASS()
class DESCENSUS_API UDesPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Descensus")
	TObjectPtr<USoundBase> FootstepSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Descensus")
	TObjectPtr<USoundBase> PointImpactSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Descensus")
	TObjectPtr<UNiagaraSystem> PointImpactEffect;
};
