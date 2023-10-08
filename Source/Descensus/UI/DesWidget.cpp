#include "UI/DesWidget.h"

#include "UI/DesHUD.h"
#include "UI/DesTooltipData.h"

void UDesWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply UDesWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const auto HUD = Cast<ADesHUD>(GetPlayerContext().GetHUD());

	HUD->UpdateWidgetUnderCursor(this);

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UDesWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	const auto HUD = Cast<ADesHUD>(GetPlayerContext().GetHUD());

	HUD->NewWidgetUnderCursor(this);
}

void UDesWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	const auto HUD = Cast<ADesHUD>(GetPlayerContext().GetHUD());

	HUD->HideTooltip();
}

FReply UDesWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FEventReply(true).NativeReply;
}

const FDesTooltipData& UDesWidget::GetTooltipData(bool bShouldSetNotDirty)
{
	return CachedTooltipData;
}
