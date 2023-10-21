#include "SDesItemWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Engine/Texture2D.h"
#include "Items/DesItemData.h"
#include "UI/DesStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemWidget::Construct(const FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	ChildSlot
	[
		SAssignNew(Box, SBox)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SAssignNew(IconImage, SImage)
			]
			+ SOverlay::Slot()
			  .VAlign(VAlign_Bottom)
			  .HAlign(HAlign_Right)
			  .Padding(0.0f, 0.0f, 5.0f, 2.0f)
			[
				SAssignNew(QuantityTextBlock, STextBlock)
				.Justification(ETextJustify::Right)
				.TextStyle(&Style->DefaultTextStyle)
				.Text(INVTEXT("1"))
			]
		]
	];
}

void SDesItemWidget::SetItem(const UDesItemInstance* ItemInstance) const
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto ItemData = ItemInstance->GetItemData();

	Box->SetHeightOverride(ItemData->Size.X * Style->CellSize);
	Box->SetWidthOverride(ItemData->Size.Y * Style->CellSize);

	IconImage->SetImage(&ItemData->IconBrush);

	SetQuantity(ItemInstance->GetQuantity(), ItemData->MaxQuantity);
}

void SDesItemWidget::SetQuantity(const int32 Quantity, const int32 MaxQuantity) const
{
	if (MaxQuantity > 1)
	{
		QuantityTextBlock->SetVisibility(EVisibility::SelfHitTestInvisible);
		static FTextFormat QuantityFormat(INVTEXT("{0}/{1}"));
		QuantityTextBlock->SetText(FText::Format(QuantityFormat, Quantity, MaxQuantity));
	}
	else
	{
		QuantityTextBlock->SetVisibility(EVisibility::Collapsed);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
