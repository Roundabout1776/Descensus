#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DesInscriptionComponent.generated.h"

class ADesCharacter;

UCLASS(ClassGroup=(Custom))
class DESCENSUS_API UDesInscriptionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Descensus|Inscription")
	FVector2D InscriptionOffset;

	UPROPERTY(Transient)
	TObjectPtr<ADesCharacter> DesCharacterOwner;

	UFUNCTION(Server, Unreliable, Category="Descensus|Inscription")
	void ServerSetInscriptionOffset(const FVector2D& InInscriptionOffset);

public:
	UDesInscriptionComponent();
	virtual void PostLoad() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetInscriptionOffset(const FVector2D& InInscriptionOffset);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInscribing() const;
};
