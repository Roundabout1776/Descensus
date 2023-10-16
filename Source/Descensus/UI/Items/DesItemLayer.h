#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Items/DesItemInstance.h"
#include "DesItemLayer.generated.h"

class UDesItemInstance;
struct FDesItemWidgetData;
class UDesItemContainerComponent;
class SDesItemLayer;
class SDesItemWidget;

UCLASS()
class DESCENSUS_API UDesItemLayer : public UWidget
{
	GENERATED_BODY()

protected:
	// TArray<TSharedPtr<SDesItemWidget>> ItemWidgetPool;
	TSharedPtr<SDesItemLayer> Widget;
	TWeakObjectPtr<UDesItemContainerComponent> ContainerToMoveFrom;
	TWeakObjectPtr<UDesItemInstance> ItemToMove;

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void BeginItemMove(UDesItemContainerComponent* InContainerToMoveFrom, UDesItemInstance* InItemToMove, const FDesItemWidgetData& ItemWidgetData, FVector2D ScreenSpacePosition);
	void EndItemMove();
	bool IsItemMoveActive() const;

	void HandlePointer(const FPointerEvent& PointerEvent) const;

	UDesItemInstance* GetItemToMove() const;
	UDesItemContainerComponent* GetContainerToMoveFrom() const;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
