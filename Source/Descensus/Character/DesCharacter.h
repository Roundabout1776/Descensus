#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DesCharacter.generated.h"

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

	UPROPERTY(VisibleInstanceOnly, Category="Descensus|Character")
	TObjectPtr<UDesCharacterAttributeSet> AttributeSet;

protected:
	virtual void BeginPlay() override;

	static FName AttributeSetName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Descensus|Character")
	TObjectPtr<UDesAbilitySystemComponent> AbilitySystemComponent;

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
	void GiveAbility(TSubclassOf<UDesGameplayAbility> AbilityClass) const;

public:
	explicit ADesCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UDesCharacterAttributeSet* GetAttributeSet() const { return AttributeSet; }

	template <class T>
	T* GetAttributeSet() const
	{
		return Cast<T>(GetAttributeSet());
	}

	UFUNCTION(BlueprintPure)
	UDesInscriptionComponent* GetInscriptionComponent() const { return InscriptionComponent; }
};
