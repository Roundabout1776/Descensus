#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemContainerWidget.generated.h"

class UDesItemContainerComponent;
class UDesInventoryComponent;
class SDesItemContainerWidget;

UCLASS()
class DESCENSUS_API UDesItemContainerWidget : public UWidget
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<SDesItemContainerWidget> ItemContainerWidget;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	TWeakObjectPtr<UDesItemContainerComponent> ItemContainerComponent;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	void AttachContainer(TObjectPtr<UDesItemContainerComponent> InItemContainerComponent);
	void OnRepGrid();
};
