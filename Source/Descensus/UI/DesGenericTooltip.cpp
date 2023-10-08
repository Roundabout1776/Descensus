#include "UI/DesGenericTooltip.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "UI/DesTooltipData.h"

void UDesGenericTooltip::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Hidden);
}

void UDesGenericTooltip::UpdateTooltipPosition(FVector2D DesiredPosition, bool bShouldAddVerticalOffset)
{
	const auto ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this) /
		UWidgetLayoutLibrary::GetViewportScale(this);
	const auto TooltipDesiredSize = GetDesiredSize() / 2.0;

	auto ActualPosition = DesiredPosition;
	ActualPosition.Y -= TooltipDesiredSize.Y;

	ActualPosition.X = FMath::Clamp(ActualPosition.X, TooltipDesiredSize.X,
	                                ViewportSize.X - TooltipDesiredSize.X);

	if (bShouldAddVerticalOffset)
	{
		ActualPosition.Y -= TooltipDesiredSize.Y / 2.0;
		if (ActualPosition.Y - TooltipDesiredSize.Y < 0.0)
		{
			ActualPosition.Y += TooltipDesiredSize.Y * 4.25;
		}
	}
	ActualPosition.Y = FMath::Clamp(ActualPosition.Y, TooltipDesiredSize.Y,
	                                ViewportSize.Y - TooltipDesiredSize.Y);

	ActualPosition -= ViewportSize / 2.0;

	SetRenderTranslation(ActualPosition);
}

void UDesGenericTooltip::SetTooltipData(FDesTooltipData TooltipData)
{
	NameLabel->SetText(TooltipData.Name);
}
