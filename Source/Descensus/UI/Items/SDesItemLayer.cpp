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
		.Expose(EjectedItemWidgetSlot)
		[
			SAssignNew(EjectedItemWidget, SDesItemWidget)
			.Visibility(EVisibility::Collapsed)
		]
	];
}

void SDesItemLayer::UpdateEjectedItemPosition(const FVector2D& MousePosition, const float DeltaTime)
{
	MousePositionLocal = this->GetTickSpaceGeometry().AbsoluteToLocal(MousePosition);

	if (EjectedItemWidget->GetVisibility() != EVisibility::Collapsed)
	{
		const auto TargetPosition = MousePositionLocal - EjectedItemOffset;
		EjectedItemWidgetSlot->SetPosition(
			FMath::InterpEaseIn(EjectedItemWidgetSlot->GetPosition(), TargetPosition,
			                    FMath::Clamp(DeltaTime * 50.0, 0.0, 1.0),
			                    2.0));
	}
}

void SDesItemLayer::BeginItemMove(const FDesItemWidgetData& ItemWidgetData)
{
	const auto Style = FDesStyle::GetDefaultStyle();
	const auto Size = FVector2D(ItemWidgetData.Size.X * Style->CellSize,
	                            ItemWidgetData.Size.Y * Style->CellSize);
	EjectedItemWidgetSlot->SetSize(Size);
	EjectedItemOffset = Size / 2.0;
	EjectedItemWidgetSlot->SetPosition(MousePositionLocal - EjectedItemOffset);
	EjectedItemWidget->SetData(ItemWidgetData);
	EjectedItemWidget->SetVisibility(EVisibility::HitTestInvisible);
}

void SDesItemLayer::EndItemMove() const
{
	EjectedItemWidget->SetVisibility(EVisibility::Collapsed);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
