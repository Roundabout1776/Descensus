#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "DesGameplayAbilityTargetDataTypes.generated.h"

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesGameplayAbilityTargetData_Actor : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FDesGameplayAbilityTargetData_Actor()
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
			const auto CustomData = static_cast<const FDesGameplayAbilityTargetData_Actor*>(Handle.
				Get(0));

			return CustomData->Actor.Get();
		}

		return nullptr;
	}
};

template <>
struct TStructOpsTypeTraits<
		FDesGameplayAbilityTargetData_Actor> : TStructOpsTypeTraitsBase2<FDesGameplayAbilityTargetData_Actor>
{
	enum
	{
		WithNetSerializer = true
	};
};

USTRUCT(BlueprintType)
struct DESCENSUS_API FDesGameplayAbilityTargetData_PrimitiveComponent : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FDesGameplayAbilityTargetData_PrimitiveComponent()
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
			const auto CustomData = static_cast<const FDesGameplayAbilityTargetData_PrimitiveComponent*>(Handle.
				Get(0));

			return CustomData->PrimitiveComponent.Get();
		}

		return nullptr;
	}
};

template <>
struct TStructOpsTypeTraits<
		FDesGameplayAbilityTargetData_PrimitiveComponent> : TStructOpsTypeTraitsBase2<
		FDesGameplayAbilityTargetData_PrimitiveComponent>
{
	enum
	{
		WithNetSerializer = true
	};
};
