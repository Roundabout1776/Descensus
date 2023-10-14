#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesCharacterScreen.generated.h"

class SDesItemContainerWidget;
class UDesInventoryComponent;
class ADesPlayerCharacter;

UCLASS()
class DESCENSUS_API UDesCharacterScreen : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SBorder> Root;
	TSharedPtr<SDesItemContainerWidget> ItemContainerWidget;
	
	TWeakObjectPtr<UDesInventoryComponent> InventoryComponent;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	void InitForCharacter(const ADesPlayerCharacter* Character);
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
