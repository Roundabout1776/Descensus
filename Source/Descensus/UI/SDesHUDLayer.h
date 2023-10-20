#pragma once

#include "CoreMinimal.h"
#include "Components/DesItemContainerComponent.h"
#include "Widgets/SCompoundWidget.h"

class UDesInventoryComponent;
class SDesItemLayer;
class SDesItemContainerWidget;
class SDesCrosshair;

class DESCENSUS_API SDesHUDLayer final : public SCompoundWidget
{
protected:
	TSharedPtr<SDesCrosshair> Crosshair;
	TSharedPtr<SDesItemContainerWidget> Inventory;
	TSharedPtr<SDesItemContainerWidget> CurrentContainer;

public:
	SLATE_BEGIN_ARGS(SDesHUDLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetCrosshairVisible(bool bNewVisible) const;

	void SetupItemSystem(const TSharedRef<SDesItemLayer>& InItemLayer,
                                          UDesInventoryComponent* InventoryComponent) const;
	void SetCurrentContainer(UDesItemContainerComponent* ItemContainerComponent) const;
};
