#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DesCharacter.generated.h"

class UAttributeSet;
class UDesGameplayAbilityRun;
class UDesGameplayAbilityJump;
class UGameplayEffect;
class UDesGameplayAbility;
class UDesCharacterAttributeSet;
class UDesMetaComponent;
class UDesInscriptionComponent;
class UDesAbilitySystemComponent;

UCLASS(Abstract)
class DESCENSUS_API ADesCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Descensus|Character")
	TObjectPtr<UDesMetaComponent> MetaComponent;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Character")
	TObjectPtr<UAudioComponent> CommonAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Magic")
	TObjectPtr<UDesInscriptionComponent> InscriptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Descensus|Character")
	TArray<TSubclassOf<UDesGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Descensus|Character")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	virtual void GiveDefaultAbilities();
	virtual void ApplyDefaultEffects();
	void GiveAbility(TSubclassOf<UDesGameplayAbility> AbilityClass) const;

public:
	TWeakObjectPtr<UDesAbilitySystemComponent> CustomASC;
	
	explicit ADesCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	UDesInscriptionComponent* GetInscriptionComponent() const { return InscriptionComponent; }

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return nullptr; }
	FORCEINLINE virtual UAttributeSet* GetAttributeSet() const { return nullptr; }

	template <class T>
	T* GetAttributeSet() const
	{
		return Cast<T>(GetAttributeSet());
	}
};
