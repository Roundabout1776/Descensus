﻿#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Layout/Visibility.h"
#include "SlotBase.h"
#include "Widgets/SWidget.h"
#include "Layout/Children.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SPanel.h"

struct FDesItemWidgetData;
class FArrangedChildren;
class FPaintArgs;
class FSlateWindowElementList;

class SDesItemContainerWidget : public SPanel
{
	SLATE_DECLARE_WIDGET(SDesItemContainerWidget, SPanel)
	class FSlot : public TWidgetSlotWithAttributeSupport<FSlot>
	{
		/** Position */
		TSlateSlotAttribute<FVector2D> Position;

		/** Size */
		TSlateSlotAttribute<FVector2D> Size;

	public:
		SLATE_SLOT_BEGIN_ARGS(FSlot, TSlotBase<FSlot>)
			SLATE_ATTRIBUTE(FVector2D, Position)
			SLATE_ATTRIBUTE(FVector2D, Size)
		SLATE_SLOT_END_ARGS()

		void SetPosition(TAttribute<FVector2D> InPosition)
		{
			Position.Assign(*this, MoveTemp(InPosition));
		}

		FVector2D GetPosition() const
		{
			return Position.Get();
		}

		void SetSize(TAttribute<FVector2D> InSize)
		{
			Size.Assign(*this, MoveTemp(InSize));
		}

		FVector2D GetSize() const
		{
			return Size.Get();
		}

		/** Default values for a slot. */
		FSlot()
			: TWidgetSlotWithAttributeSupport<FSlot>()
			  , Position(*this, FVector2D::ZeroVector)
			  , Size(*this, FVector2D(1.0f, 1.0f))
		{
		}

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArg);
		static void RegisterAttributes(FSlateWidgetSlotAttributeInitializer& AttributeInitializer);
	};

protected:
	TPanelChildren<FSlot> Children;
	TSlateAttribute<FIntVector> GridSizeAttribute;
	int32 CurrentItemWidgetIndex{};
	bool bIsTelegraphVisible{};
	FVector2D TelegraphPosition{};
	FVector2D TelegraphSize{};

	virtual FVector2D ComputeDesiredSize(float) const override;

	static FSlot::FSlotArguments Slot();

	using FScopedWidgetSlotArguments = TPanelChildren<FSlot>::FScopedWidgetSlotArguments;

	FScopedWidgetSlotArguments AddSlot();

public:
	SLATE_BEGIN_ARGS(SDesItemContainerWidget)
			: _GridSize({1, 1, 0})
		{
			_Visibility = EVisibility::Visible;
		}

		SLATE_ATTRIBUTE(FIntVector, GridSize)
		SLATE_SLOT_ARGUMENT(FSlot, Slots)
	SLATE_END_ARGS()

	SDesItemContainerWidget();

	void Construct(const FArguments& InArgs);

	virtual void
	OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FChildren* GetChildren() override;

	void SetTelegraphPosition(FVector2D Position);
	void SetTelegraphSize(FVector2D Size);
	void SetTelegraphVisible(bool bIsVisible);
	void SetGridSize(FIntVector InGridSize);
	FIntVector GetGridSize() const;
	void AddItem(FIntVector2 Position, const FDesItemWidgetData& Data);
	void CollapseAllItems();
};