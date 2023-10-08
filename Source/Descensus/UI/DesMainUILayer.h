#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DesMainUILayer.generated.h"

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

public:
	void SetInscriptionOverlayMaterial(UMaterialInstanceDynamic* Material) const;
	void SetCrosshairVisible(bool bNewVisible) const;

	UFUNCTION()
	void HandleControlMappingsRebuilt();
};
