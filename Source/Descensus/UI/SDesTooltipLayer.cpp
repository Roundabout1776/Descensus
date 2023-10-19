#include "SDesTooltipLayer.h"

#include "DesLogging.h"
#include "DesStyle.h"
#include "DesTooltipData.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesTooltipLayer::Construct(const FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	SetVisibility(EVisibility::Hidden);

	ChildSlot
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Top)
	[
		 SAssignNew(Tooltip, SBorder)
		 .Padding(Style->Padding)
		 .BorderImage(&Style->CommonBox)
		 [
			 SAssignNew(TooltipHeader, STextBlock)
			 .Justification(ETextJustify::Center)
			 .TextStyle(&Style->DefaultTextStyle)
			 .Text(INVTEXT("Tooltip Header"))
		 ]
	];
}

void SDesTooltipLayer::SetTooltipData(const FDesTooltipData& TooltipData)
{
	CachedViewportSize = GetParentWidget()->GetTickSpaceGeometry().GetLocalSize();
	TooltipHeader->SetText(TooltipData.Header);
}

void SDesTooltipLayer::SetTooltipPosition(const FVector2D& InPosition, bool bShouldAddVerticalOffset)
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
	SetRenderTransform(Transform);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
