#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "DesItemContainerComponent.generated.h"

class UDesItemData;
class UDesItemInstance;

USTRUCT(BlueprintType)
struct FItemContainerEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector2 Position{};

	UPROPERTY()
	TObjectPtr<UDesItemInstance> ItemInstance;
};

USTRUCT(BlueprintType)
struct FItemContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FItemContainerEntry, FItemContainer>(Items, DeltaParams, *this);
	}

	void AddItem(TSubclassOf<UDesItemData> InItemDataClass);
	void AddItem(TObjectPtr<UDesItemInstance> InItemInstance);

	void RemoveItem(TSubclassOf<UDesItemData> InItemDataClass);
	void RemoveItem(TObjectPtr<UDesItemInstance> InItemInstance);

	TArray<FItemContainerEntry>& GetItemsRef() { return Items; }

	TArray<TObjectPtr<UDesItemInstance>> GetAllInstancesWithTag(FGameplayTag Tag);

	TArray<TObjectPtr<UDesItemInstance>> GetAllAvailableInstancesOfType(TSubclassOf<UDesItemData> InItemDataClass);

protected:
	UPROPERTY()
	TArray<FItemContainerEntry> Items;
};

template <>
struct TStructOpsTypeTraits<FItemContainer> : TStructOpsTypeTraitsBase2<FItemContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

DECLARE_DELEGATE(FOnRepGridSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	FOnRepGridSignature OnRepGridDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_Grid)
	FItemContainer Grid;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Inventory")
	TArray<TSubclassOf<UDesItemData>> DefaultItems;

	UDesItemContainerComponent();
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnRep_Grid(const FItemContainer& OldGrid);
};
