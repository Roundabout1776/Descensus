#include "SDesHUDLayer.h"

#include "DesStyle.h"
#include "SDesCrosshair.h"
#include "Components/DesItemContainerComponent.h"
#include "Items/SDesItemContainerWidget.h"
#include "Items/SDesItemLayer.h"
#include "Player/DesInventoryComponent.h"

void SDesHUDLayer::Construct(const FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	SetVisibility(EVisibility::SelfHitTestInvisible);

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		  .HAlign(HAlign_Center)
		  .VAlign(VAlign_Center)
		[
			SAssignNew(Crosshair, SDesCrosshair)
			.Size(20.0)
			.Alignment(FVector2D(0.5, 0.5))
			.Thickness(2.0)
			.ColorAndOpacity(FColor::White)
			.Visibility(EVisibility::Hidden)
		]
		+ SOverlay::Slot()
		  .HAlign(HAlign_Right)
		  .VAlign(VAlign_Top)
		[
			SNew(SBorder)
			.Padding(Style->Padding)
			.BorderImage(&Style->CommonBox)
			[
				SAssignNew(Inventory, SDesItemContainerWidget)
			]
		]
		+ SOverlay::Slot()
		  .HAlign(HAlign_Right)
		  .VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.Padding(Style->Padding)
			.BorderImage(&Style->CommonBox)
			[
				SAssignNew(CurrentContainer, SDesItemContainerWidget)
			]
		]
	];
}

void SDesHUDLayer::SetCrosshairVisible(const bool bNewVisible) const
{
	Crosshair->SetVisibility(bNewVisible ? EVisibility::HitTestInvisible : EVisibility::Hidden);
}

void SDesHUDLayer::SetupItemSystem(const TSharedRef<SDesItemLayer>& InItemLayer,
                                   UDesInventoryComponent* InventoryComponent) const
{
	Inventory->AttachToItemContainerComponent(Cast<UDesItemContainerComponent>(InventoryComponent));
	Inventory->SetItemLayer(InItemLayer);

	CurrentContainer->SetItemLayer(InItemLayer);
	CurrentContainer->SetVisibility(EVisibility::Collapsed);
}

void SDesHUDLayer::SetCurrentContainer(UDesItemContainerComponent* ItemContainerComponent) const
{
	if (ItemContainerComponent)
	{
		CurrentContainer->AttachToItemContainerComponent(ItemContainerComponent);
		CurrentContainer->SetVisibility(EVisibility::Visible);
	}
	else
	{
		CurrentContainer->DetachFromItemContainerComponent();
		CurrentContainer->SetVisibility(EVisibility::Collapsed);
	}
}
