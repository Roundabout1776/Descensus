#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"

struct FDesTooltipData;
class SCanvas;

class DESCENSUS_API SDesTooltipLayer final : public SCompoundWidget
{
	FVector2D CachedViewportSize;
	TSharedPtr<SBorder> Tooltip;
	TSharedPtr<STextBlock> TooltipHeader;

public:
	SLATE_BEGIN_ARGS(SDesTooltipLayer)
		{
		}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetTooltipData(const FDesTooltipData& TooltipData);
	void SetTooltipPosition(const FVector2D& InPosition, bool bShouldAddVerticalOffset);
};
