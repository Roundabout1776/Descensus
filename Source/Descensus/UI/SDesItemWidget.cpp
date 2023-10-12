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
	
	Box->SetHeightOverride(ItemData->Volume.X * Style->CellSize);
	Box->SetWidthOverride(ItemData->Volume.Y * Style->CellSize);

	FSlateBrush Brush;
	Brush.SetResourceObject(ItemInstance->ItemDataClass.GetDefaultObject()->Icon.LoadSynchronous());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	IconImage->SetImage(&Brush);

	QuantityTextBlock->SetText(FText::FromString(FString::FromInt(ItemInstance->Quantity)));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
