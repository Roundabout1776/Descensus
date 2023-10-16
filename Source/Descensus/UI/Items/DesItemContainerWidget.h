#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemContainerWidget.generated.h"

class UDesItemLayer;
class UDesItemInstance;
struct FDesItemWidgetData;
class UDesItemContainerComponent;
class SDesItemContainerWidget;

UCLASS()
class DESCENSUS_API UDesItemContainerWidget : public UWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UDesItemContainerComponent> ItemContainerComponent;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UDesItemLayer> ItemLayer;
	
	TSharedPtr<SDesItemContainerWidget> Widget;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void SetItemLayer(UDesItemLayer* InItemLayer);
	void AttachToItemContainerComponent(UDesItemContainerComponent* InItemContainerComponent);
	FReply OnItemContainerClicked(const FGeometry& Geometry, const FPointerEvent& MouseEvent, const FIntVector2& Coords) const;
	static FDesItemWidgetData GetItemWidgetData(const UDesItemInstance* ItemInstance);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
