#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DesHUD.generated.h"

class UImage;
class ADesInscriptionCanvas;
class ADesPlayerCharacter;
class UDesMainUILayer;
class UDesWidget;
class UWidget;
class UOverlay;
class UDesGenericTooltip;
class UDesMetaComponent;
class SDesShortcutsPanel;
struct FDesTooltipData;

UENUM(BlueprintType)
enum class ECursorTarget : uint8
{
	None,
	Actor,
	Widget
};

UCLASS(Abstract)
class DESCENSUS_API ADesHUD : public AHUD
{
	GENERATED_BODY()

	FVector2D GetDesiredTooltipPositionForActor(const AActor* Actor) const;

protected:
	virtual void BeginPlay() override;

	/* Widgets */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDesGenericTooltip> GenericTooltip;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|UI")
	TSubclassOf<UDesGenericTooltip> GenericTooltipClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDesMainUILayer> MainUILayer;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|UI")
	TSubclassOf<UDesMainUILayer> MainUILayerClass;

	/* Actor that handles capture of inscription FX onto a texture */
	UPROPERTY(VisibleInstanceOnly, Transient, Category="Descensus|Inscription")
	TObjectPtr<ADesInscriptionCanvas> InscriptionCanvas;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Inscription")
	TSubclassOf<ADesInscriptionCanvas> InscriptionCanvasClass;

public:
	UPROPERTY(BlueprintReadOnly)
	ECursorTarget CurrentCursorTarget = ECursorTarget::None;

	void NewWidgetUnderCursor(UDesWidget* Widget);
	void UpdateWidgetUnderCursor(UDesWidget* Widget);

	void NewActorUnderCursor(const AActor* Actor, const UDesMetaComponent* MetaComponent);
	void UpdateActorUnderCursor(const AActor* Actor) const;

	void ShowTooltip(FDesTooltipData TooltipData, FVector2D DesiredPosition, bool bShouldAddVerticalOffset);
	void HideTooltip();

	FORCEINLINE ADesInscriptionCanvas* GetInscriptionCanvas() const { return InscriptionCanvas; }

	void InitMainUILayer(const ADesPlayerCharacter* Character);

	void LookStarted();
	void LookCompleted();
};
