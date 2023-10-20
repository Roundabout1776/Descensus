#include "SDesHUDLayer.h"

#include "SDesCrosshair.h"

void SDesHUDLayer::Construct(const FArguments& InArgs)
{
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
	];
}

void SDesHUDLayer::SetCrosshairVisible(const bool bNewVisible) const
{
	Crosshair->SetVisibility(bNewVisible ? EVisibility::HitTestInvisible : EVisibility::Hidden);
}
