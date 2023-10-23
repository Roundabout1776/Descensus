#include "SDesPopupLayer.h"

#include "DesLogging.h"
#include "DesStyle.h"
#include "DesTooltipData.h"
#include "Items/DesItemData.h"
#include "SDesItemWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesPopupLayer::Construct(const FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	SetVisibility(EVisibility::HitTestInvisible);

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		  .HAlign(HAlign_Left)
		  .VAlign(VAlign_Top)
		[

			SAssignNew(Tooltip, SBorder)
		 .Padding(Style->Padding)
		 .BorderImage(&Style->CommonBox)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[

					SAssignNew(TooltipHeader, STextBlock)
			 .Justification(ETextJustify::Center)
			 .TextStyle(&Style->DefaultTextStyle)
			 .Text(INVTEXT("Tooltip Header"))
				]
				+ SVerticalBox::Slot()
				[

					SAssignNew(TooltipDescription, STextBlock)
			 .Justification(ETextJustify::Center)
			 .TextStyle(&Style->DefaultTextStyle)
			 .Text(INVTEXT("Tooltip Description"))
				]
			]
		]
		+ SOverlay::Slot()
		  .HAlign(HAlign_Left)
		  .VAlign(VAlign_Top)
		[

			SAssignNew(EjectedItemWidget, SDesItemWidget)
		]
	];
}

void SDesPopupLayer::SetTooltipData(const FDesTooltipData& TooltipData)
{
	CachedViewportSize = GetParentWidget()->GetTickSpaceGeometry().GetLocalSize();
	TooltipHeader->SetText(TooltipData.Header);
	if (TooltipData.Description.IsEmpty())
	{
		TooltipDescription->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		TooltipDescription->SetText(TooltipData.Description);
		TooltipDescription->SetVisibility(EVisibility::HitTestInvisible);
	}
	SlatePrepass();
}

void SDesPopupLayer::SetTooltipPosition(const FVector2D& InPosition, const bool bShouldAddVerticalOffset) const
{
	const auto TooltipDesiredSize = GetDesiredSize();

	auto ActualPosition = InPosition;
	ActualPosition.Y -= TooltipDesiredSize.Y;
	ActualPosition.X -= TooltipDesiredSize.X / 2.0;

	ActualPosition.X = FMath::Clamp(ActualPosition.X, 0,
	                                CachedViewportSize.X - TooltipDesiredSize.X);

	if (bShouldAddVerticalOffset)
	{
		ActualPosition.Y -= TooltipDesiredSize.Y / 4.0;
		if (ActualPosition.Y < 0.0)
		{
			ActualPosition.Y += TooltipDesiredSize.Y * 2.5;
		}
	}

	ActualPosition.Y = FMath::Clamp(ActualPosition.Y, 0,
	                                CachedViewportSize.Y - TooltipDesiredSize.Y);

	FSlateRenderTransform Transform;
	Transform.SetTranslation(ActualPosition);
	Tooltip->SetRenderTransform(Transform);
}

void SDesPopupLayer::SetTooltipVisible(const bool bIsVisible) const
{
	Tooltip->SetVisibility(bIsVisible ? EVisibility::HitTestInvisible : EVisibility::Hidden);
}

void SDesPopupLayer::SetEjectedItemPosition(const FVector2D& MousePosition)
{
	CachedMousePosition = MousePosition;

	if (EjectedItemWidget->GetVisibility() == EVisibility::HitTestInvisible)
	{
		const auto TargetPosition = CachedMousePosition - EjectedItemOffset;

		FSlateRenderTransform Transform;
		Transform.SetTranslation(TargetPosition);
		EjectedItemWidget->SetRenderTransform(Transform);
	}
}

void SDesPopupLayer::UpdateEjectedItemQuantity(const int32 Quantity, const int32 MaxQuantity) const
{
	EjectedItemWidget->SetQuantity(Quantity, MaxQuantity);
}

void SDesPopupLayer::SetEjectedItem(const UDesItemInstance* ItemInstance)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto ItemData = ItemInstance->GetItemData();
	const auto Size = FVector2D(ItemData->Size.X * Style->CellSize,
	                            ItemData->Size.Y * Style->CellSize);

	EjectedItemOffset = Size / 2.0;

	FSlateRenderTransform Transform;
	Transform.SetTranslation(CachedMousePosition - EjectedItemOffset);
	EjectedItemWidget->SetRenderTransform(Transform);

	EjectedItemWidget->SetItem(ItemInstance);
}

void SDesPopupLayer::SetEjectedItemVisible(const bool bIsVisible) const
{
	EjectedItemWidget->SetVisibility(bIsVisible ? EVisibility::HitTestInvisible : EVisibility::Hidden);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
