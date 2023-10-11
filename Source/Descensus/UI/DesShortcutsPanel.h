#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DesShortcutsPanel.generated.h"

class UInputAction;
class SDesShortcutSlot;
class SDesShortcutsPanel;
class ADesPlayerController;

USTRUCT(BlueprintType)
struct FDesShortcutSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
	bool bFixed = false;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bFixed"))
	TObjectPtr<UTexture2D> FixedIconTexture;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditInstanceOnly)
	float Size = 70.0f;
};

UCLASS()
class DESCENSUS_API UDesShortcutsPanel final : public UWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<SHorizontalBox> ShortcutsContainer;
	TArray<TSharedPtr<SDesShortcutSlot>> Shortcuts;

	UPROPERTY(EditAnywhere, Category="Descensus|UI")
	TArray<FDesShortcutSlotData> SlotDataArray;

public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	void UpdateInputMappings(const ADesPlayerController* PlayerController);
};
