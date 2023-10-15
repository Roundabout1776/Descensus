#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesItemContainerWidget.generated.h"

class UDesItemContainerComponent;
class SDesItemContainerWidget;

UCLASS()
class DESCENSUS_API UDesItemContainerWidget : public UWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TWeakObjectPtr<UDesItemContainerComponent> ItemContainerComponent;
	
	TSharedPtr<SDesItemContainerWidget> Widget;
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void AttachToItemContainerComponent(UDesItemContainerComponent* InItemContainerComponent);
	void OnItemContainerClicked(const UE::Math::TIntVector2<int>& IntVector2) const;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
