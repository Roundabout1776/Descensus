#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DesMainUILayer.generated.h"

class UDesItemLayer;
class UDesCharacterScreen;
class UDesItemContainerWidget;
class UDesInventoryComponent;
class UInputAction;
class SDesShortcutsPanel;
class UOverlay;
class ADesPlayerController;
class UDesShortcutsPanel;
class UImage;
class UDesShortcuts;
class UDesStatusBar;

UCLASS(Abstract)
class DESCENSUS_API UDesMainUILayer : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> RootOverlay;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> InscriptionOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUserWidget> Crosshair;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesStatusBar> StatusBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesShortcutsPanel> ShortcutsPanel;

	// UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	// TObjectPtr<UDesCharacterScreen> CharacterScreen;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesItemLayer> ItemLayer;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesItemContainerWidget> Inventory;
	
	void SetCrosshairVisible(bool bNewVisible) const;

	UFUNCTION()
	void HandleControlMappingsRebuilt();
};
