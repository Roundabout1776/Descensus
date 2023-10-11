#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "DesInventoryComponent.generated.h"

class UDesItemData;
class UDesItemInstance;

USTRUCT(BlueprintType)
struct FInventoryGridItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector2 Position{};

	UPROPERTY()
	TObjectPtr<UDesItemInstance> ItemInstance;
};

USTRUCT(BlueprintType)
struct FInventoryGrid : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInventoryGridItem, FInventoryGrid>(Items, DeltaParams, *this);
	}

	void AddItem(TSubclassOf<UDesItemData> InItemDataClass);
	void AddItem(TObjectPtr<UDesItemInstance> InItemInstance);

	void RemoveItem(TSubclassOf<UDesItemData> InItemDataClass);
	void RemoveItem(TObjectPtr<UDesItemInstance> InItemInstance);

	TArray<FInventoryGridItem>& GetItemsRef() { return Items; }

	TArray<TObjectPtr<UDesItemInstance>> GetAllInstancesWithTag(FGameplayTag Tag);

	TArray<TObjectPtr<UDesItemInstance>> GetAllAvailableInstancesOfType(TSubclassOf<UDesItemData> InItemDataClass);

protected:
	UPROPERTY()
	TArray<FInventoryGridItem> Items;
};

template <>
struct TStructOpsTypeTraits<FInventoryGrid> : TStructOpsTypeTraitsBase2<FInventoryGrid>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

DECLARE_DELEGATE(FOnRepGridSignature)

UCLASS(ClassGroup=(Descensus), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	FOnRepGridSignature OnRepGridDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_Grid)
	FInventoryGrid Grid;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Inventory")
	TArray<TSubclassOf<UDesItemData>> DefaultItems;

	UDesInventoryComponent();
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnRep_Grid(const FInventoryGrid& OldGrid);
};
