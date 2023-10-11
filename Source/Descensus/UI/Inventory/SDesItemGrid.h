#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class SImage;
class STextBlock;
class UTexture2D;

class DESCENSUS_API SDesItemGrid : public SCompoundWidget
{
	FIntVector2 GridSize{};

	const float CellSize = 50.0f;
	const float Padding = 18.0f;
	const FSlateBrush* BackgroundBrush{};

public:
	SLATE_BEGIN_ARGS(SDesItemGrid)
		{
		}

		SLATE_ARGUMENT(FIntVector2, GridSize)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
};
