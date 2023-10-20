#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemContainerWidget.generated.h"

class SDesItemLayer;
struct FItemContainerEntry;
class ADesPlayerState;
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
	FDelegateHandle OnAnyChangesDelegateHandle;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly)
	FIntVector PreviewGridSize;
#endif

	UPROPERTY(Transient)
	TWeakObjectPtr<UDesItemContainerComponent> ItemContainerComponent;

	TSharedPtr<SDesItemLayer> ItemLayer;

	TSharedPtr<SDesItemContainerWidget> Widget;

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void SetItemLayer(const TSharedRef<SDesItemLayer>& InItemLayer);
	void AttachToItemContainerComponent(UDesItemContainerComponent* InItemContainerComponent);
	void DetachFromItemContainerComponent();
	void OnyAnyChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const;
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	FReply HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	void HandleMouseLeave(const FPointerEvent& PointerEvent) const;
	FIntVector2 ClampCoords(const FIntVector2& InCoords) const;
	FIntVector2 GetCoordsUnderPointerForSize(const FGeometry& Geometry, const FPointerEvent& PointerEvent,
	                                         const FIntVector2& Size) const;
	FIntVector2 GetCoordsUnderPointer(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	static FDesItemWidgetData GetItemWidgetData(const UDesItemInstance* ItemInstance);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
