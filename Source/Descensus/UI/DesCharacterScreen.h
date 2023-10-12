#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesCharacterScreen.generated.h"

class UDesInventoryComponent;
class ADesPlayerCharacter;
class SDesItemContainerWidget;

UCLASS()
class DESCENSUS_API UDesCharacterScreen : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SBorder> Root;
	TSharedPtr<SDesItemContainerWidget> Inventory;
	
	TWeakObjectPtr<UDesInventoryComponent> InventoryComponent;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	void InitForCharacter(const ADesPlayerCharacter* Character);
	void OnRepGrid();
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
