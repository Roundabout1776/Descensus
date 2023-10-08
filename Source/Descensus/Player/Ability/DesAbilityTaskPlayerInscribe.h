#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DesAbilityTaskPlayerInscribe.generated.h"

class ADesPlayerController;
class ADesInscriptionCanvas;
class UDesInscriptionComponent;

DECLARE_MULTICAST_DELEGATE(FPlayerInscribeDelegate)

UCLASS()
class DESCENSUS_API UDesAbilityTaskPlayerInscribe : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(Transient, DuplicateTransient)
	TWeakObjectPtr<UDesInscriptionComponent> InscriptionComponent;

	UPROPERTY(Transient, DuplicateTransient)
	TWeakObjectPtr<ADesInscriptionCanvas> InscriptionCanvas;

	UPROPERTY(Transient, DuplicateTransient)
	TWeakObjectPtr<ADesPlayerController> PlayerController;

protected:
	virtual void Activate() override;

	void PerformAction() const;

public:
	virtual void OnDestroy(bool bInOwnerFinished) override;

	FTimerHandle PerformActionTimerHandle;

	static UDesAbilityTaskPlayerInscribe* PlayerInscribe(UGameplayAbility* OwningAbility,
	                                                     UDesInscriptionComponent* InInscriptionComponent,
	                                                     ADesInscriptionCanvas* InInscriptionCanvas,
	                                                     ADesPlayerController* InPlayerController);
};
