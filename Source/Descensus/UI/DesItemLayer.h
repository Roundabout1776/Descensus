#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemLayer.generated.h"

class SDesItemLayer;
struct FDesItemWidgetData;
class SDesItemWidget;

UCLASS()
class DESCENSUS_API UDesItemLayer : public UWidget
{
	GENERATED_BODY()

protected:
	// TArray<TSharedPtr<SDesItemWidget>> ItemWidgetPool;
	TSharedPtr<SDesItemLayer> Widget;

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void BeginItemDragDrop(const FDesItemWidgetData& ItemWidgetData, FVector2D ScreenSpacePosition) const;
	void EndItemDragDrop() const;
	bool IsItemDragDropActive() const;

	void HandlePointer(const FPointerEvent& PointerEvent) const;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
