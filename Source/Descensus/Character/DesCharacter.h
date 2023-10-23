#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DesCharacter.generated.h"

class UAttributeSet;
class UDesGameplayAbility_Run;
class UDesGameplayAbilityJump;
class UGameplayEffect;
class UDesGameplayAbility;
class UDesCharacterAttributeSet;
class UDesMetaComponent;
class UDesInscriptionComponent;
class UDesAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FDesStepSound
{
	GENERATED_BODY()

	float Time;
	bool bSkip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Descensus|Character")
	bool bEnableStepSound = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition="bEnableStepSound"))
	float MinSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition="bEnableStepSound"))
	float MinRunningSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition="bEnableStepSound"))
	float TimeWalking = 435.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition="bEnableStepSound"))
	float TimeRunning = 335.0f;
};

UCLASS(Abstract)
class DESCENSUS_API ADesCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	void TickStepSound(float DeltaTime);

protected:
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

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Character")
	FDesStepSound StepSound;

	virtual void BeginPlay() override;
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
