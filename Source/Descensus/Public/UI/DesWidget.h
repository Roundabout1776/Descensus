#pragma once

#include "CoreMinimal.h"
#include "DesTooltipData.h"
#include "Blueprint/UserWidget.h"
#include "DesWidget.generated.h"

struct FDesTooltipData;

UCLASS(Abstract)
class DESCENSUS_API UDesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	FDesTooltipData CachedTooltipData;
	bool bIsTooltipDirty;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	virtual const FDesTooltipData& GetTooltipData(bool bShouldSetNotDirty);

	FORCEINLINE bool IsTooltipDirty() const { return bIsTooltipDirty; }
};
