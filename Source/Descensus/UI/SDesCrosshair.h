#pragma once

class DESCENSUS_API SDesCrosshair final : public SBox
{
public:
	SLATE_BEGIN_ARGS(SDesCrosshair)
			: _Thickness(1)
			  , _Size(1)
			  , _Alignment(0.5f)
			  , _ColorAndOpacity(FLinearColor::White)
		{
		}

		SLATE_ARGUMENT(float, Thickness)
		SLATE_ARGUMENT(float, Size)
		SLATE_ARGUMENT(FVector2D, Alignment)
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		Thickness = InArgs._Thickness;
		Alignment = InArgs._Alignment;
		ColorAndOpacity = InArgs._ColorAndOpacity;

		SBox::Construct(SBox::FArguments().WidthOverride(InArgs._Size).HeightOverride(InArgs._Size));
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override
	{
		const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
		const FLinearColor Tint = ColorAndOpacity.Get(FLinearColor::White).GetColor(InWidgetStyle);

		++LayerId;
		TArray<FVector2D> VerticalLine = {
			FVector2D(Alignment.X * LocalSize.X, 0.0f), FVector2D(Alignment.X * LocalSize.X, LocalSize.Y)
		};
		TArray<FVector2D> HorizontalLine = {
			FVector2D(0.0f, Alignment.Y * LocalSize.Y), FVector2D(LocalSize.X, Alignment.Y * LocalSize.Y)
		};

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			VerticalLine,
			ESlateDrawEffect::None,
			Tint,
			true,
			Thickness
		);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			HorizontalLine,
			ESlateDrawEffect::None,
			Tint,
			true,
			Thickness
		);

		return SBox::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
		                     bParentEnabled);
	}

private:
	TAttribute<FSlateColor> ColorAndOpacity;
	float Thickness{};
	FVector2D Alignment;
};
