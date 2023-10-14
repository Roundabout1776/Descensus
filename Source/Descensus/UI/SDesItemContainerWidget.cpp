#include "SDesItemContainerWidget.h"

#include "DesStyle.h"
#include "SDesItemWidget.h"
#include "Types/PaintArgs.h"
#include "Layout/ArrangedChildren.h"

SLATE_IMPLEMENT_WIDGET(SDesItemContainerWidget)

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemContainerWidget::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	FSlateWidgetSlotAttributeInitializer Initializer = SLATE_ADD_PANELCHILDREN_DEFINITION(
		AttributeInitializer, Children);
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

	TAlignmentWidgetSlotMixin<FSlot>::ConstructMixin(SlotOwner, MoveTemp(InArg));
}


SDesItemContainerWidget::SDesItemContainerWidget()
	: Children(this, GET_MEMBER_NAME_CHECKED(SDesItemContainerWidget, Children))
{
	SetCanTick(false);
	bCanSupportFocus = false;
}

void SDesItemContainerWidget::Construct(const SDesItemContainerWidget::FArguments& InArgs)
{
	const auto Style = FDesStyle::GetDefaultStyle();

	TelegraphSize = FVector2D(Style->CellSize, Style->CellSize);
	GridSize = InArgs._GridSize;
	Children.AddSlots(MoveTemp(const_cast<TArray<FSlot::FSlotArguments>&>(InArgs._Slots)));
}

SDesItemContainerWidget::FSlot::FSlotArguments SDesItemContainerWidget::Slot()
{
	return FSlot::FSlotArguments(MakeUnique<FSlot>());
}

SDesItemContainerWidget::FScopedWidgetSlotArguments SDesItemContainerWidget::AddSlot()
{
	return FScopedWidgetSlotArguments{MakeUnique<FSlot>(), Children, INDEX_NONE};
}

void SDesItemContainerWidget::ClearChildren()
{
	Children.Empty();
}

int32 SDesItemContainerWidget::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
{
	return Children.Remove(SlotWidget);
}

void SDesItemContainerWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	if (Children.Num() > 0)
	{
		for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
		{
			const SDesItemContainerWidget::FSlot& CurChild = Children[ChildIndex];
			const FVector2D Size = CurChild.GetSize();

			//Handle HAlignment
			FVector2D Offset(0.0f, 0.0f);

			switch (CurChild.GetHorizontalAlignment())
			{
			case HAlign_Center:
				Offset.X = -Size.X / 2.0f;
				break;
			case HAlign_Right:
				Offset.X = -Size.X;
				break;
			case HAlign_Fill:
			case HAlign_Left:
				break;
			}

			//handle VAlignment
			switch (CurChild.GetVerticalAlignment())
			{
			case VAlign_Bottom:
				Offset.Y = -Size.Y;
				break;
			case VAlign_Center:
				Offset.Y = -Size.Y / 2.0f;
				break;
			case VAlign_Top:
			case VAlign_Fill:
				break;
			}

			// Add the information about this child to the output list (ArrangedChildren)
			ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
				// The child widget being arranged
				CurChild.GetWidget(),
				// Child's local position (i.e. position within parent)
				CurChild.GetPosition() + Offset,
				// Child's size
				Size
			));
		}
	}
}

int32 SDesItemContainerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
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

	if (bShowTelegraph)
	{
		const auto TelegraphGeometry = AllottedGeometry.MakeChild(TelegraphSize,
		                                                          FSlateLayoutTransform(TelegraphPosition))
		                                               .ToPaintGeometry();
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, TelegraphGeometry, &Style->ItemContainerTelegraphBrush);
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
	// Canvas widgets have no desired size -- their size is always determined by their container
	return FVector2D::ZeroVector;
}

FChildren* SDesItemContainerWidget::GetChildren()
{
	return &Children;
}

void SDesItemContainerWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SPanel::OnMouseEnter(MyGeometry, MouseEvent);
}

void SDesItemContainerWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SPanel::OnMouseLeave(MouseEvent);
	bShowTelegraph = false;
}

FReply SDesItemContainerWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D MouseLcal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	FVector2D MouseLocal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	MouseLocal /= MyGeometry.Size;
	MouseLocal *= FVector2D(GridSize.X, GridSize.Y);
	const FIntVector2 Coords{static_cast<int>(MouseLocal.X), static_cast<int>(MouseLocal.Y)};
	bShowTelegraph = Coords.X >= 0 && Coords.X < GridSize.X && Coords.Y >= 0 && Coords.Y < GridSize.Y;
	if (bShowTelegraph)
	{
		TelegraphPosition = FVector2D{
			Coords.X * (MyGeometry.Size.X / GridSize.X), Coords.Y * (MyGeometry.Size.Y / GridSize.Y)
		};
	}
	return SPanel::OnMouseMove(MyGeometry, MouseEvent);
}

void SDesItemContainerWidget::AddItem(FIntVector2 Position, FIntVector2 Size, const FSlateBrush* Texture)
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
	Slot.SetSize(FVector2D(Size.X * Style->CellSize, Size.Y * Style->CellSize));
	Slot.SetPosition(FVector2D(Position.X * Style->CellSize, Position.Y * Style->CellSize));
	const auto Child = StaticCastSharedRef<SDesItemWidget>(Slot.GetWidget());
	Child->SetFromInstance(Position, Size, Texture);

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
