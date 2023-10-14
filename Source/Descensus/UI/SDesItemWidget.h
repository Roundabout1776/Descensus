#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UDesItemInstance;
class UDesItemData;
class SImage;
class STextBlock;
class UTexture2D;
class SBox;

class DESCENSUS_API SDesItemWidget : public SCompoundWidget
{
	// FIntVector2 Size{};
	TSharedPtr<SBox> Box;
	TSharedPtr<SImage> IconImage;
	TSharedPtr<STextBlock> QuantityTextBlock;

public:
	FVector2D Position{};
	
	SLATE_BEGIN_ARGS(SDesItemWidget)
		{
		}

	SLATE_END_ARGS()
	
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	void Construct(const FArguments& InArgs);

	void SetFromInstance(FIntVector2 InPosition, FIntVector2 Size, const FSlateBrush* Brush);
};
