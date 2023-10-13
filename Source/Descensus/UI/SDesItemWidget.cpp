#include "SDesItemWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Engine/Texture2D.h"
#include "UI/DesStyle.h"
#include "Items/DesItemInstance.h"
#include "SDesItemContainerWidget.h"
#include "Items/DesItemData.h"

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
			  .Padding(0.0f, 0.0f, 0.0f, 0.0f)
			[
				SAssignNew(QuantityTextBlock, STextBlock)
				.Justification(ETextJustify::Right)
				.TextStyle(&Style->DefaultTextStyle)
				.Text(INVTEXT("0"))
			]
		]
	];
}

void SDesItemWidget::SetFromInstance(const UDesItemInstance* ItemInstance) const
{
	if (!IsValid(ItemInstance) || !IsValid(ItemInstance->ItemDataClass))
		return;

	const auto ItemData = ItemInstance->ItemDataClass.GetDefaultObject();

	const auto Style = FDesStyle::GetDefaultStyle();
	
	Box->SetHeightOverride(ItemData->Size.X * Style->CellSize);
	Box->SetWidthOverride(ItemData->Size.Y * Style->CellSize);

	IconImage->SetImage(&ItemInstance->GetItemData()->IconBrush);

	QuantityTextBlock->SetText(FText::FromString(FString::FromInt(ItemInstance->Quantity)));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
