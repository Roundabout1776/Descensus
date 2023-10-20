#pragma once

#include "CoreMinimal.h"
#include "DesCharacter.h"
#include "DesNPC.generated.h"

class UAttributeSet;

UCLASS()
class DESCENSUS_API ADesNPC : public ADesCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category="Descensus|Character")
	TObjectPtr<UDesCharacterAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, Category="Descensus|Character")
	TObjectPtr<UDesAbilitySystemComponent> ASC;

protected:
	virtual void BeginPlay() override;

public:
	explicit ADesNPC(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE virtual UAttributeSet* GetAttributeSet() const override;
};
