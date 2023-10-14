#include "SDesItemWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Engine/Texture2D.h"
#include "UI/DesStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SDesItemWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const auto FG = 10;
	return FReply::Handled();
}

void SDesItemWidget::Construct(const FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	SetVisibility(EVisibility::Visible);

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
			  .Padding(0.0f, 0.0f, 5.0f, 0.0f)
			[
				SAssignNew(QuantityTextBlock, STextBlock)
				.Justification(ETextJustify::Right)
				.TextStyle(&Style->DefaultTextStyle)
				.Text(INVTEXT("1"))
			]
		]
	];
}

void SDesItemWidget::SetFromInstance(FIntVector2 InPosition, FIntVector2 Size, const FSlateBrush* Brush)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	// Position = FVector2D(InPosition.X * Style->CellSize, InPosition.Y * Style->CellSize);

	Box->SetHeightOverride(Size.X * Style->CellSize);
	Box->SetWidthOverride(Size.Y * Style->CellSize);

	// IconImage->SetImage(&Style->TestBrush);
	IconImage->SetImage(Brush);

	SetVisibility(EVisibility::Visible);

	// ComputeDesiredSize()
	// SetDesiredSize({Size.X * Style->CellSize, Size.Y * Style->CellSize});
	// const auto B = ChildSlot.GetWidget()->GetDesiredSize();
	// Invalidate(EInvalidateWidgetReason::Layout);

	// QuantityTextBlock->SetText(FText::FromString(FString::FromInt(ItemInstance->Quantity)));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
