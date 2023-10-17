#pragma once

#include "CoreMinimal.h"
#include "DesItemContainerComponent.h"
#include "DesInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemToMoveChangedSignature, const UDesItemInstance*)

UCLASS(ClassGroup=(Descensus), meta=(BlueprintSpawnableComponent))
class DESCENSUS_API UDesInventoryComponent : public UDesItemContainerComponent
{
	GENERATED_BODY()
	
public:
	FOnItemToMoveChangedSignature OnEjectedItemChanged;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_EjectedItem, Category="Descensus|Character")
	TObjectPtr<UDesItemInstance> EjectedItem;
	
	UDesInventoryComponent();

	UFUNCTION(Server, Reliable)
	void ServerDestroyItem(UDesItemInstance* InItemInstance);
	
	UFUNCTION(Server, Reliable)
	void ServerDestroyEjectedItem();

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerMoveEjectedItem(UDesItemContainerComponent* Container, FIntVector2 Coords);

	UFUNCTION(Server, Reliable)
	void ServerEjectItem(UDesItemContainerComponent* Container, UDesItemInstance* InItemInstance);
	
	UFUNCTION()
	void OnRep_EjectedItem() const;
};
