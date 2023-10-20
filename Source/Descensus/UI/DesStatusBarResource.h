#pragma once

#include "CoreMinimal.h"
#include "DesTooltipData.h"
#include "Blueprint/UserWidget.h"
#include "DesStatusBarResource.generated.h"

class UObjectProperty;
class UProgressBar;

UCLASS()
class DESCENSUS_API UDesStatusBarResource : public UUserWidget
{
	GENERATED_BODY()

	FDesTooltipData CachedTooltipData;
	bool bIsTooltipDirty;
	FText Text;
	float CurrentPercent;
	float TargetPercent;
	float Value = 100.0;
	float MaxValue = 100.0;
	float GetPercent() const { return Value / MaxValue; }

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	virtual const FDesTooltipData& GetTooltipData(bool bSetNotDirty);

	void SetTextAndValues(const FText& InText, float InValue, float InMaxValue);
	void SetValues(float InValue, float InMaxValue);
	FORCEINLINE bool IsTooltipDirty() const { return bIsTooltipDirty; }
};
