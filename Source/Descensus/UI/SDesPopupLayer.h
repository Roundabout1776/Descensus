#pragma once

#include "DesTooltipData.h"

struct FDesTooltipData;
class UDesItemInstance;
class SDesItemWidget;
class IDesTooltip;

class DESCENSUS_API FDesTooltipMetaData final : public ISlateMetaData
{
public:
	SLATE_METADATA_TYPE(FDesTooltipMetaData, ISlateMetaData)

	explicit FDesTooltipMetaData(const TSharedRef<IDesTooltip>& InTooltip) : Tooltip(InTooltip.ToWeakPtr())
	{
	}

	FDesTooltipMetaData(const FDesTooltipMetaData&) = delete;
	FDesTooltipMetaData& operator=(const FDesTooltipMetaData&) = delete;

	TWeakPtr<IDesTooltip> Tooltip;
};

class DESCENSUS_API IDesTooltip
{
	FDesTooltipData CachedTooltipData;
	bool bIsDirty = true;
	
protected:
	virtual FDesTooltipData& GetCachedTooltipDataMutable() { return CachedTooltipData; }

	void SetTooltipDirty(const bool bNewIsDirty)
	{
		bIsDirty = bNewIsDirty;
	}

	virtual void UpdateCachedTooltipData() = 0;

	template <typename WidgetWithTooltip, typename = TEnableIf<
		          TIsDerivedFrom<WidgetWithTooltip, SWidget>::Value && TIsDerivedFrom<
			          WidgetWithTooltip, IDesTooltip>::Value,
		          bool>>
	void AddTooltipMetaData(const TSharedRef<WidgetWithTooltip>& InSharedThis)
	{
		InSharedThis->AddMetadata(MakeShared<FDesTooltipMetaData>(InSharedThis));
	}

public:
	virtual ~IDesTooltip() = default;

	const FDesTooltipData& GetCachedTooltipData()
	{
		if (IsTooltipDirty())
		{
			UpdateCachedTooltipData();
			SetTooltipDirty(false);
		}
		return CachedTooltipData;
	}

	virtual bool ShouldShowTooltip() { return true; }
	bool IsTooltipDirty() const { return bIsDirty; }
};

class DESCENSUS_API SDesPopupLayer final : public SCompoundWidget
{
protected:
	FVector2D CachedViewportSize;
	FVector2D CachedMousePosition;

	TSharedPtr<SBorder> Tooltip;
	TSharedPtr<STextBlock> TooltipHeader;
	TSharedPtr<STextBlock> TooltipDescription;

	FVector2D EjectedItemOffset;
	TSharedPtr<SDesItemWidget> EjectedItemWidget;

public:
	SLATE_BEGIN_ARGS(SDesPopupLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetTooltipData(const FDesTooltipData& TooltipData);
	void SetTooltipPosition(const FVector2D& InPosition, bool bShouldAddVerticalOffset) const;
	void SetTooltipVisible(bool bIsVisible) const;

	void SetEjectedItemPosition(const FVector2D& MousePosition);
	void UpdateEjectedItemQuantity(int32 Quantity, int32 MaxQuantity) const;
	void SetEjectedItem(const UDesItemInstance* ItemInstance);
	void SetEjectedItemVisible(bool bIsVisible) const;
};
