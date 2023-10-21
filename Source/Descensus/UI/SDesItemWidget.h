#pragma once

#include "CoreMinimal.h"
#include "Items/DesItemInstance.h"
#include "Widgets/SCompoundWidget.h"

class UDesItemInstance;
class UDesItemData;
class SImage;
class STextBlock;
class SBox;

class DESCENSUS_API SDesItemWidget final : public SCompoundWidget
{
	TSharedPtr<SBox> Box;
	TSharedPtr<SImage> IconImage;
	TSharedPtr<STextBlock> QuantityTextBlock;

public:
	SLATE_BEGIN_ARGS(SDesItemWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetItem(const UDesItemInstance* ItemInstance) const;
	void SetQuantity(int32 Quantity, int32 MaxQuantity) const;
};
