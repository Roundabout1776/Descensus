#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "DesGameplayAbilityTargetDataTypes.generated.h"

USTRUCT(BlueprintType)
struct DESCENSUS_API FGameplayAbilityTargetDataSingleActor : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FGameplayAbilityTargetDataSingleActor()
	{
	}

	TWeakObjectPtr<AActor> Actor;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << Actor;
		bOutSuccess = true;
		return true;
	}

	static AActor* GetActor(const FGameplayAbilityTargetDataHandle& Handle)
	{
		const auto Data = Handle.Get(0);

		if (!Data)
		{
			return nullptr;
		}

		if (Data->GetScriptStruct() == StaticStruct())
		{
			const auto CustomData = static_cast<const FGameplayAbilityTargetDataSingleActor*>(Handle.
				Get(0));

			return CustomData->Actor.Get();
		}

		return nullptr;
	}
};

template <>
struct TStructOpsTypeTraits<
		FGameplayAbilityTargetDataSingleActor> : TStructOpsTypeTraitsBase2<FGameplayAbilityTargetDataSingleActor>
{
	enum
	{
		WithNetSerializer = true
	};
};

USTRUCT(BlueprintType)
struct DESCENSUS_API FGameplayAbilityTargetDataPrimitiveComponent : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FGameplayAbilityTargetDataPrimitiveComponent()
	{
	}

	TWeakObjectPtr<UPrimitiveComponent> PrimitiveComponent;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << PrimitiveComponent;
		bOutSuccess = true;
		return true;
	}

	static UPrimitiveComponent* GetPrimitiveComponent(const FGameplayAbilityTargetDataHandle& Handle)
	{
		const auto Data = Handle.Get(0);

		if (!Data)
		{
			return nullptr;
		}

		if (Data->GetScriptStruct() == StaticStruct())
		{
			const auto CustomData = static_cast<const FGameplayAbilityTargetDataPrimitiveComponent*>(Handle.
				Get(0));

			return CustomData->PrimitiveComponent.Get();
		}

		return nullptr;
	}
};

template <>
struct TStructOpsTypeTraits<
		FGameplayAbilityTargetDataPrimitiveComponent> : TStructOpsTypeTraitsBase2<
		FGameplayAbilityTargetDataPrimitiveComponent>
{
	enum
	{
		WithNetSerializer = true
	};
};
