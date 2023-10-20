#pragma once

#include "CoreMinimal.h"
#include "DesTooltipData.h"
#include "Types/ISlateMetaData.h"
#include "Widgets/SCompoundWidget.h"

class SDesTooltipLayer;
class SCanvas;
class IDesTooltip;

class DESCENSUS_API FDesTooltipMetaData final : public ISlateMetaData
{
public:
	SLATE_METADATA_TYPE(FDesTooltipMetaData, ISlateMetaData)

	explicit FDesTooltipMetaData(IDesTooltip* InTooltip) : Tooltip(InTooltip)
	{
	}

	FDesTooltipMetaData(const FDesTooltipMetaData&) = delete;
	FDesTooltipMetaData& operator=(const FDesTooltipMetaData&) = delete;

	IDesTooltip* Tooltip{};
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

	bool IsTooltipDirty() const { return bIsDirty; }

	virtual void UpdateCachedTooltipData() = 0;

	void AddTooltipMetaData(SWidget* Widget)
	{
		Widget->AddMetadata(MakeShared<FDesTooltipMetaData>(this));
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
};

class DESCENSUS_API SDesTooltipLayer final : public SCompoundWidget
{
	FVector2D CachedViewportSize;
	TSharedPtr<SBorder> Tooltip;
	TSharedPtr<STextBlock> TooltipHeader;

public:
	SLATE_BEGIN_ARGS(SDesTooltipLayer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetTooltipData(const FDesTooltipData& TooltipData);
	void SetTooltipPosition(const FVector2D& InPosition, bool bShouldAddVerticalOffset);
};
