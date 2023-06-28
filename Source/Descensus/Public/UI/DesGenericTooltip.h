#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DesGenericTooltip.generated.h"

class UTextBlock;
struct FDesTooltipData;

UCLASS(Abstract)
class DESCENSUS_API UDesGenericTooltip : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> NameLabel;

	void UpdateTooltipPosition(FVector2D DesiredPosition, bool bShouldAddVerticalOffset);

	void SetTooltipData(FDesTooltipData TooltipData);
};
