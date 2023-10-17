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

	UPROPERTY(VisibleInstanceOnly)
	FIntVector2 Position{};

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<UDesItemInstance> ItemInstance;
};

USTRUCT(BlueprintType)
struct FItemContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	friend struct FItemContainerEntry;
	friend class UDesItemContainerComponent;

	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FItemContainerEntry, FItemContainer>(Items, DeltaParams, *this);
	}

protected:
	UPROPERTY(VisibleInstanceOnly)
	TArray<FItemContainerEntry> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UDesItemContainerComponent> OwnerComponent;
};

template <>
struct TStructOpsTypeTraits<FItemContainer> : TStructOpsTypeTraitsBase2<FItemContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

UCLASS(Blueprintable, ClassGroup=(Descensus), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()
	
	friend struct FItemContainer;
	friend class UDesInventoryComponent;

protected:
	/* Replicated list of items. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Descensus|Items")
	FItemContainer Array;

	bool bGridDirty;

	virtual void BeginPlay() override;

	void OnItemAdded(const FItemContainerEntry& Entry);
	void OnItemChanged(const FItemContainerEntry& Entry);
	void OnItemRemoved(const FItemContainerEntry& Entry);

	void OnAnyChanges();
	void FillGrid(FIntVector2 Coords, FIntVector2 Size, int32 InIndex = 0);
	void RebuildGrid();

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAddedSignature, const FItemContainerEntry&)

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemChangedSignature, const FItemContainerEntry&)

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemRemovedSignature, const FItemContainerEntry&)

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnyChangesSignature, const TArray<FItemContainerEntry>&)

	FOnItemAddedSignature OnItemAddedDelegate;
	FOnItemAddedSignature OnItemChangedDelegate;
	FOnItemAddedSignature OnItemRemovedDelegate;
	FOnAnyChangesSignature OnAnyChangesDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Items")
	FIntVector GridSize{5, 5, 0};

	/* Locally maintained fast-access item grid. */
	TArray<int32> Grid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Items")
	TArray<TSubclassOf<UDesItemData>> DefaultItems;

	UDesItemContainerComponent();
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FItemContainerEntry>& GetItemsRef() { return Array.Items; }

	UFUNCTION(BlueprintCallable)
	bool AddItemAuto(UDesItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItemByInstance(UDesItemInstance* InItemInstance);

	UDesItemInstance* GetItemInstance(FIntVector2 Coords);

	virtual bool CanInteractWithContainer(UDesItemContainerComponent* Container) { return Container == this; }

	int32 IntVectorToIndex(FIntVector2 Coords) const;
	static int32 GridValueToItemsIndex(int32 Value);
	int32 GridCoordsToItemsIndex(FIntVector2 Coords) const;
};
