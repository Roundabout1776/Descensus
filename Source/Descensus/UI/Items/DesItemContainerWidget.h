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
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	FReply HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	FReply HandleMouseLeave(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	FIntVector2 ClampCoords(const FIntVector2& InCoords) const;
	FIntVector2 GetCoordsUnderPointerForSize(const FGeometry& Geometry, const FPointerEvent& PointerEvent, const FIntVector2& Size) const;
	FIntVector2 GetCoordsUnderPointer(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	static FDesItemWidgetData GetItemWidgetData(const UDesItemInstance* ItemInstance);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
