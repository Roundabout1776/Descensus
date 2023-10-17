#include "SDesItemLayer.h"

#include "SDesItemWidget.h"
#include "UI/DesStyle.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesItemLayer::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::HitTestInvisible);
	ChildSlot
	[
		SAssignNew(Canvas, SCanvas)
		+ SCanvas::Slot()
		.Expose(ItemWidgetSlot)
		[
			SAssignNew(ItemWidget, SDesItemWidget)
			.Visibility(EVisibility::Collapsed)
		]
	];
}

void SDesItemLayer::HandlePointer(const FPointerEvent& PointerEvent, const float DeltaTime)
{
	PointerPosition = this->GetTickSpaceGeometry().AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());
	if (ItemWidget->GetVisibility() != EVisibility::Collapsed)
	{
		const auto TargetPosition = PointerPosition - EjectedItemOffset;

		ItemWidgetSlot->SetPosition(
			FMath::InterpEaseIn(ItemWidgetSlot->GetPosition(), TargetPosition, FMath::Clamp(DeltaTime * 50.0, 0.0, 1.0), 2.0));
	}
}

void SDesItemLayer::BeginItemMove(const FDesItemWidgetData& ItemWidgetData)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto Size = FVector2D(ItemWidgetData.Size.X * Style->CellSize,
	                            ItemWidgetData.Size.Y * Style->CellSize);
	ItemWidgetSlot->SetSize(Size);
	EjectedItemOffset = Size / 2.0;
	ItemWidgetSlot->SetPosition(PointerPosition - EjectedItemOffset);
	ItemWidget->SetData(ItemWidgetData);
	ItemWidget->SetVisibility(EVisibility::HitTestInvisible);
}

void SDesItemLayer::EndItemMove() const
{
	ItemWidget->SetVisibility(EVisibility::Collapsed);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
