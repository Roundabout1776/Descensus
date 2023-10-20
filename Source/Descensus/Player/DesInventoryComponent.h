#pragma once

#include "CoreMinimal.h"
#include "Components/DesItemContainerComponent.h"
#include "DesInventoryComponent.generated.h"

class ADesPlayerCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemToMoveChangedSignature, const UDesItemInstance*)

UCLASS(ClassGroup=(Descensus), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesInventoryComponent : public UDesItemContainerComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_EjectedItem, Category="Descensus|Character")
	TObjectPtr<UDesItemInstance> EjectedItem;

	TWeakObjectPtr<ADesPlayerCharacter> CharacterOwner;

public:
	FOnItemToMoveChangedSignature OnEjectedItemChanged;

	UDesInventoryComponent();
	virtual void InitializeComponent() override;

	bool CanInteractWithContainer(const UDesItemContainerComponent* Container);

	UFUNCTION(Server, Reliable)
	void ServerDestroyItem(UDesItemInstance* InItemInstance);

	UFUNCTION(Server, Reliable)
	void ServerDestroyEjectedItem();

	UFUNCTION(Server, Reliable)
	void ServerMoveEjectedItem(UDesItemContainerComponent* Container, FIntVector2 Coords);

	UFUNCTION(Server, Reliable)
	void ServerEjectItem(UDesItemContainerComponent* Container, UDesItemInstance* InItemInstance);

	UFUNCTION()
	void OnRep_EjectedItem();

	void SetEjectedItem(UDesItemInstance* ItemInstance);
	FORCEINLINE UDesItemInstance* GetEjectedItem() const { return EjectedItem; }
};
