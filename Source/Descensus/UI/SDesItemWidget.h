#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UDesItemInstance;
class UDesItemData;
class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class SImage;
class STextBlock;
class UTexture2D;
class SBox;

class DESCENSUS_API SDesItemWidget : public SCompoundWidget
{
	TSharedPtr<SBox> Box;
	TSharedPtr<SImage> IconImage;
	TSharedPtr<STextBlock> QuantityTextBlock;

public:
	SLATE_BEGIN_ARGS(SDesItemWidget)
		{
		}

		SLATE_ATTRIBUTE(TObjectPtr<UTexture2D>, FixedIconTexture)
		SLATE_ATTRIBUTE(TObjectPtr<UInputAction>, InputAction)
		SLATE_ATTRIBUTE(float, SizeOverride)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetFromInstance(const UDesItemInstance* ItemInstance) const;
};
