#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DesAbilityInputID.h"
#include "GameplayTagContainer.h"
#include "DesInputConfig.generated.h"

enum class EDesAbilityInputID : uint8;
class UDesGameplayAbility;
class UInputAction;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesTaggedInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta=(Categories="Input"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesAbilityInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta=(Categories="Input"))
	EDesAbilityInputID AbilityInputID = EDesAbilityInputID::None;

	/** Use ETriggerEvent::Started instead of ETriggerEvent::Triggered as ability pressed event */
	UPROPERTY(EditDefaultsOnly)
	bool bUseStartedTriggerEvent = false;
};

UCLASS()
class DESCENSUS_API UDesInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InInputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(TitleProperty="InputAction"))
	TArray<FDesTaggedInputAction> TaggedInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(TitleProperty="InputAction"))
	TArray<FDesAbilityInputAction> AbilityInputActions;
};
