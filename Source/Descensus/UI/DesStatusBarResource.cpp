#include "UI/DesStatusBarResource.h"

#include "Components/ProgressBar.h"
#include "UI/DesTooltipData.h"

void UDesStatusBarResource::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentPercent = FMath::FInterpConstantTo(CurrentPercent, TargetPercent, InDeltaTime, 1.0f);
	ProgressBar->SetPercent(CurrentPercent);
}

const FDesTooltipData& UDesStatusBarResource::GetTooltipData(bool bSetNotDirty)
{
	if (IsTooltipDirty())
	{
		CachedTooltipData.Header = FText::Format(Text, FMath::RoundToFloat(Value), FMath::RoundToFloat(MaxValue));
	}
	if (bSetNotDirty)
	{
		bIsTooltipDirty = false;
	}
	return CachedTooltipData;
}

void UDesStatusBarResource::SetTextAndValues(const FText& InText, float InValue, float InMaxValue)
{
	Text = InText;
	SetValues(InValue, InMaxValue);
}

void UDesStatusBarResource::SetValues(float InValue, float InMaxValue)
{
	Value = InValue;
	MaxValue = InMaxValue;
	TargetPercent = GetPercent();
	bIsTooltipDirty = true;
}
