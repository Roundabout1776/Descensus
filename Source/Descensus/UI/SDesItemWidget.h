#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UDesItemInstance;
class UDesItemData;
class SImage;
class STextBlock;
class UTexture2D;
class SBox;

struct DESCENSUS_API FDesItemWidgetData
{
	FIntVector2 Size{};
	int32 Quantity{};
	int32 MaxQuantity{};
	const FSlateBrush* Brush = nullptr;
};

class DESCENSUS_API SDesItemWidget final : public SCompoundWidget
{
	// FIntVector2 Size{};
	TSharedPtr<SBox> Box;
	TSharedPtr<SImage> IconImage;
	TSharedPtr<STextBlock> QuantityTextBlock;

public:
	SLATE_BEGIN_ARGS(SDesItemWidget)
		{
		}

		SLATE_ARGUMENT(FDesItemWidgetData, Data)

	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	void SetDataAndMakeVisible(const FDesItemWidgetData& Data);
};
