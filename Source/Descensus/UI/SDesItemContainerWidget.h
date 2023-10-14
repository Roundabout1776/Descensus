#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Layout/Visibility.h"
#include "SlotBase.h"
#include "Widgets/SWidget.h"
#include "Layout/Children.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SPanel.h"

class FArrangedChildren;
class FPaintArgs;
class FSlateWindowElementList;

class SDesItemContainerWidget : public SPanel
{
	SLATE_DECLARE_WIDGET_API(SDesItemContainerWidget, SPanel, DESCENSUS_API)

public:
	class FSlot : public TWidgetSlotWithAttributeSupport<FSlot>
	{
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

	public:
#if WITH_EDITORONLY_DATA
		UE_DEPRECATED(5.0, "Direct access to PositionAttr is now deprecated. Use the getter or setter.")
		TSlateDeprecatedTAttribute<FVector2D> PositionAttr;
		UE_DEPRECATED(5.0, "Direct access to SizeAttr is now deprecated. Use the getter or setter.")
		TSlateDeprecatedTAttribute<FVector2D> SizeAttr;
#endif

	public:
		/** Default values for a slot. */
		FSlot()
			: TWidgetSlotWithAttributeSupport<FSlot>()
			  , Position(*this, FVector2D::ZeroVector)
			  , Size(*this, FVector2D(1.0f, 1.0f))
		{
		}

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArg);
		static void RegisterAttributes(FSlateWidgetSlotAttributeInitializer& AttributeInitializer);

	private:
		/** Position */
		TSlateSlotAttribute<FVector2D> Position;

		/** Size */
		TSlateSlotAttribute<FVector2D> Size;
	};

	FIntVector GridSize{1, 1, 0};

	SLATE_BEGIN_ARGS(SDesItemContainerWidget)
		{
			_Visibility = EVisibility::Visible;
		}

		SLATE_ARGUMENT(FIntVector, GridSize)

		SLATE_SLOT_ARGUMENT(FSlot, Slots)

	SLATE_END_ARGS()

	SDesItemContainerWidget();

	void Construct(const FArguments& InArgs);

	static FSlot::FSlotArguments Slot();

	using FScopedWidgetSlotArguments = TPanelChildren<FSlot>::FScopedWidgetSlotArguments;

	FScopedWidgetSlotArguments AddSlot();

	int32 RemoveSlot(const TSharedRef<SWidget>& SlotWidget);

	void ClearChildren();

	virtual void
	OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FChildren* GetChildren() override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	int32 CurrentItemWidgetIndex{};
	bool bShowTelegraph{};
	FVector2D TelegraphPosition{};
	FVector2D TelegraphSize{};
	void AddItem(FIntVector2 Position, FIntVector2 Size, const FSlateBrush* Texture);
	void CollapseAllItems();

protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

	TPanelChildren<FSlot> Children;
};
