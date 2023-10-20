﻿#include "SDesItemContainerWidget.h"

#include "SDesItemWidget.h"
#include "Types/PaintArgs.h"
#include "Layout/ArrangedChildren.h"
#include "UI/DesStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SLATE_IMPLEMENT_WIDGET(SDesItemContainerWidget)

void SDesItemContainerWidget::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	FSlateWidgetSlotAttributeInitializer Initializer = SLATE_ADD_PANELCHILDREN_DEFINITION(
		AttributeInitializer, Children);
	// SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, GridSizeAttribute, EInvalidateWidgetReason::Paint);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "GridSize", GridSizeAttribute,
	                                                EInvalidateWidgetReason::Paint);
	FSlot::RegisterAttributes(Initializer);
}

void SDesItemContainerWidget::FSlot::RegisterAttributes(FSlateWidgetSlotAttributeInitializer& AttributeInitializer)
{
	TWidgetSlotWithAttributeSupport::RegisterAttributes(AttributeInitializer);
	SLATE_ADD_SLOT_ATTRIBUTE_DEFINITION_WITH_NAME(FSlot, AttributeInitializer, "Slot.Size", Size,
	                                              EInvalidateWidgetReason::Paint);
	SLATE_ADD_SLOT_ATTRIBUTE_DEFINITION_WITH_NAME(FSlot, AttributeInitializer, "Slot.Position", Position,
	                                              EInvalidateWidgetReason::Paint)
		.UpdatePrerequisite("Slot.Size");
}

void SDesItemContainerWidget::FSlot::Construct(const FChildren& SlotOwner, FSlotArguments&& InArg)
{
	TWidgetSlotWithAttributeSupport<FSlot>::Construct(SlotOwner, MoveTemp(InArg));
	if (InArg._Size.IsSet())
	{
		Size.Assign(*this, MoveTemp(InArg._Size));
	}
	if (InArg._Position.IsSet())
	{
		Position.Assign(*this, MoveTemp(InArg._Position));
	}
}

SDesItemContainerWidget::SDesItemContainerWidget()
	: Children(this, GET_MEMBER_NAME_CHECKED(SDesItemContainerWidget, Children))
	  , GridSizeAttribute(*this)
{
	SetCanTick(false);
	bCanSupportFocus = false;
}

void SDesItemContainerWidget::Construct(const FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	TelegraphSize = FVector2D(Style->CellSize, Style->CellSize);
	GridSizeAttribute.Assign(*this, InArgs._GridSize);
	Children.AddSlots(MoveTemp(const_cast<TArray<FSlot::FSlotArguments>&>(InArgs._Slots)));

	AddTooltipMetaData(this);
}

SDesItemContainerWidget::FSlot::FSlotArguments SDesItemContainerWidget::Slot()
{
	return FSlot::FSlotArguments(MakeUnique<FSlot>());
}

SDesItemContainerWidget::FScopedWidgetSlotArguments SDesItemContainerWidget::AddSlot()
{
	return FScopedWidgetSlotArguments{MakeUnique<FSlot>(), Children, INDEX_NONE};
}

void SDesItemContainerWidget::UpdateCachedTooltipData()
{
	auto& Data = GetCachedTooltipDataMutable();

	Data.Header = INVTEXT("Complex Header");
}

void SDesItemContainerWidget::OnArrangeChildren(const FGeometry& AllottedGeometry,
                                                FArrangedChildren& ArrangedChildren) const
{
	if (Children.Num() > 0)
	{
		for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
		{
			const FSlot& CurChild = Children[ChildIndex];
			const FVector2D Size = CurChild.GetSize();

			// Add the information about this child to the output list (ArrangedChildren)
			ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
				// The child widget being arranged
				CurChild.GetWidget(),
				// Child's local position (i.e. position within parent)
				CurChild.GetPosition(),
				// Child's size
				Size
			));
		}
	}
}

int32 SDesItemContainerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                       const FSlateRect& MyCullingRect,
                                       FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                       const FWidgetStyle& InWidgetStyle,
                                       bool bParentEnabled) const
{
	const auto PaintGeometry = AllottedGeometry.ToPaintGeometry();
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto Size = AllottedGeometry.GetLocalSize();
	static TArray<FVector2D> PointsTemp;

	for (int X = 0; X <= Size.X; X += Style->CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({static_cast<float>(X), 0});
		PointsTemp.Add({static_cast<float>(X), Size.Y});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, PaintGeometry, PointsTemp,
		                             ESlateDrawEffect::None, Style->ItemGridColor, false, 1.0f);
	}

	for (int Y = 0; Y <= Size.Y; Y += Style->CellSize)
	{
		PointsTemp.Reset();
		PointsTemp.Add({0, static_cast<float>(Y)});
		PointsTemp.Add({Size.X, static_cast<float>(Y)});

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, PaintGeometry, PointsTemp,
		                             ESlateDrawEffect::None, Style->ItemGridColor, false, 1.0f);
	}

	if (bIsTelegraphVisible)
	{
		const auto TelegraphGeometry = AllottedGeometry.MakeChild(TelegraphSize,
		                                                          FSlateLayoutTransform(TelegraphPosition))
		                                               .ToPaintGeometry();
		const auto Tint = Style->ItemContainerTelegraphBrush.TintColor.GetSpecifiedColor();
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, TelegraphGeometry, &Style->ItemContainerTelegraphBrush,
		                           ESlateDrawEffect::None, Tint);
	}

	/* Canvas stuff. */

	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	this->ArrangeChildren(AllottedGeometry, ArrangedChildren);

	// Because we paint multiple children, we must track the maximum layer id that they produced in case one of our parents
	// wants to an overlay for all of its contents.
	int32 MaxLayerId = LayerId;

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	const FPaintArgs NewArgs = Args.WithNewParent(this);

	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];

		if (!IsChildWidgetCulled(MyCullingRect, CurWidget))
		{
			const int32 CurWidgetsMaxLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, MyCullingRect,
			                                                           OutDrawElements, MaxLayerId + 1, InWidgetStyle,
			                                                           bForwardedEnabled);

			MaxLayerId = FMath::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
		else
		{
		}
	}

	return MaxLayerId;
}

FVector2D SDesItemContainerWidget::ComputeDesiredSize(float) const
{
	const auto Style = FDesStyle::GetDefaultStyle();
	return FVector2D(GridSizeAttribute.Get().X * Style->CellSize, GridSizeAttribute.Get().Y * Style->CellSize);
}

FChildren* SDesItemContainerWidget::GetChildren()
{
	return &Children;
}

void SDesItemContainerWidget::SetTelegraphPosition(FVector2D Position)
{
	TelegraphPosition = Position;
}

void SDesItemContainerWidget::SetTelegraphSize(FVector2D Size)
{
	TelegraphSize = Size;
}

void SDesItemContainerWidget::SetTelegraphVisible(bool bIsVisible)
{
	bIsTelegraphVisible = bIsVisible;
}

void SDesItemContainerWidget::SetGridSize(FIntVector InGridSize)
{
	GridSizeAttribute.Set(*this, InGridSize);
}

FIntVector SDesItemContainerWidget::GetGridSize() const
{
	return GridSizeAttribute.Get();
}

void SDesItemContainerWidget::AddItem(const FIntVector2 Position, const FDesItemWidgetData& Data)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	if (CurrentItemWidgetIndex >= Children.Num())
	{
		AddSlot()
		[
			SNew(SDesItemWidget)
		];
	}
	auto& Slot = Children[CurrentItemWidgetIndex];
	Slot.SetSize(FVector2D(Data.Size.X * Style->CellSize, Data.Size.Y * Style->CellSize));
	Slot.SetPosition(FVector2D(Position.X * Style->CellSize, Position.Y * Style->CellSize));
	const auto Child = StaticCastSharedRef<SDesItemWidget>(Slot.GetWidget());
	Child->SetData(Data);
	Child->SetVisibility(EVisibility::HitTestInvisible);

	CurrentItemWidgetIndex++;
}

void SDesItemContainerWidget::CollapseAllItems()
{
	Children.ForEachWidget([](SWidget& Widget)
	{
		Widget.SetVisibility(EVisibility::Collapsed);
	});
	CurrentItemWidgetIndex = 0;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
