#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

class UDesRuneData;
struct FDesStrokePoint;

DECLARE_DELEGATE_OneParam(FOnResolutionChangedSignature, int)
DECLARE_DELEGATE_OneParam(FOnAddPointSignature, const FDesStrokePoint&)

class DESCENSUSEDITOR_API SDesRuneDataWidget : public SCompoundWidget
{
	const int GridSize = 64;

	FVector2D PointUnderCursorNormalized;

	TAttribute<const TArray<FDesStrokePoint>*> PointsRaw;
	TAttribute<const TArray<FDesStrokePoint>*> Points;

	// const TArray<FDesStrokePoint>* PointsRaw = nullptr;
	// const TArray<FDesStrokePoint>* Points = nullptr;

	FOnResolutionChangedSignature OnResolutionChangedDelegate;
	FOnAddPointSignature OnAddPointDelegate;

	FSlateBrush BackgroundBrush;

	static FVector2D NormalizedToLocal(const FGeometry& AllottedGeometry, FVector2D InNormalized);
	static FVector2D LocalToNormalized(const FGeometry& AllottedGeometry, FVector2D InLocal);
	FIntVector2 NormalizedToInt(FVector2D InNormalized) const;
	FVector2D IntToNormalized(const FIntVector2& InInt) const;

public:
	SLATE_BEGIN_ARGS(SDesRuneDataWidget)
			: _PointsRaw(nullptr)
			  , _Points(nullptr)
		{
		}

		SLATE_ATTRIBUTE(const TArray<FDesStrokePoint>*, PointsRaw)
		SLATE_ATTRIBUTE(const TArray<FDesStrokePoint>*, Points)
		SLATE_EVENT(FOnResolutionChangedSignature, OnResolutionChangedDelegate)
		SLATE_EVENT(FOnAddPointSignature, OnAddPointDelegate)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

	virtual FReply OnMouseButtonDown(const FGeometry& AllottedGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& AllottedGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& AllottedGeometry, const FPointerEvent& MouseEvent) override;
};
