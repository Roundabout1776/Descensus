#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemGrid.generated.h"

class SDesItemGrid;

UCLASS()
class DESCENSUS_API UDesItemGrid : public UWidget
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<SDesItemGrid> ItemGrid;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
