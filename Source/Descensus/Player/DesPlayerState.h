#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DesPlayerState.generated.h"

class UDesPlayerAttributeSet;
class UDesAbilitySystemComponent;

UCLASS()
class DESCENSUS_API ADesPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, Category="Descensus|Character")
	TObjectPtr<UDesPlayerAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, Category="Descensus|Character")
	TObjectPtr<UDesAbilitySystemComponent> ASC;
	
	ADesPlayerState();
	
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE virtual UDesPlayerAttributeSet* GetAttributeSet() const;
};
