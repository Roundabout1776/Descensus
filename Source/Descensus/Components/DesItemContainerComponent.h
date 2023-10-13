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

	void PostReplicatedAdd(const struct FItemContainer& InArraySerializer);
	void PostReplicatedChange(const struct FItemContainer& InArraySerializer);
	void PreReplicatedRemove(const struct FItemContainer& InArraySerializer);
};

DECLARE_DELEGATE_OneParam(FOnItemAddedSignature, const FItemContainerEntry&)
DECLARE_DELEGATE_OneParam(FOnItemChangedSignature, const FItemContainerEntry&)
DECLARE_DELEGATE_OneParam(FOnItemRemovedSignature, const FItemContainerEntry&)

USTRUCT(BlueprintType)
struct FItemContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	friend struct FItemContainerEntry;
	friend class UDesItemContainerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FItemContainerEntry, FItemContainer>(Items, DeltaParams, *this);
	}

protected:
	FOnItemAddedSignature OnItemAddedDelegate;
	FOnItemChangedSignature OnItemChangedDelegate;
	FOnItemRemovedSignature OnItemRemovedDelegate;
	
	UPROPERTY(VisibleInstanceOnly)
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

UCLASS(Blueprintable, ClassGroup=(Descensus), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/* Replicated list of items. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Descensus|Items")
	FItemContainer Array;
	
	virtual void BeginPlay() override;

	void FillGrid(FIntVector2 Coords, FIntVector2 Size, UDesItemInstance* ItemInstance = nullptr);

public:
	FOnItemAddedSignature& GetOnItemAddedDelegate() { return Array.OnItemAddedDelegate; }
	FOnItemChangedSignature& GetOnItemChangedDelegate() { return Array.OnItemChangedDelegate; }
	FOnItemRemovedSignature& GetOnItemRemovedDelegate() { return Array.OnItemRemovedDelegate; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Items")
	FIntVector GridSize{5, 5, 0};

	/* Locally maintained fast-access item grid. */
	UPROPERTY()
	TArray<TObjectPtr<UDesItemInstance>> Grid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Descensus|Items")
	TArray<TSubclassOf<UDesItemData>> DefaultItems;

	UDesItemContainerComponent();
	virtual void InitializeComponent() override;

	TArray<FItemContainerEntry>& GetItemsRef() { return Array.Items; }

	UFUNCTION(BlueprintCallable)
	bool AddItemAuto(UDesItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UDesItemInstance* InItemInstance);
};
