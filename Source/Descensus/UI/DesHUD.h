#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DesHUD.generated.h"

class SDesHUDLayer;
class SDesItemLayer;
class SDesTooltipLayer;
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
	static inline constexpr int32 MainLayerZ = 0;
	static inline constexpr int32 HUDLayerZ = 1;
	static inline constexpr int32 ItemLayerZ = 4;
	static inline constexpr int32 TooltipLayerZ = 5;

	TSharedPtr<SDesHUDLayer> HUDLayer;
	TSharedPtr<SDesItemLayer> ItemLayer;
	TSharedPtr<SDesTooltipLayer> TooltipLayer;

	TSharedPtr<FSlateUser> SlateUser;

	TWeakPtr<SWidget> LastTooltipWidgetUnderCursor;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDesMainUILayer> MainUILayer;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|UI")
	TSubclassOf<UDesMainUILayer> MainUILayerClass;

	/* Actor that handles capture of inscription FX onto a texture */
	UPROPERTY(VisibleInstanceOnly, Transient, Category="Descensus|Inscription")
	TObjectPtr<ADesInscriptionCanvas> InscriptionCanvas;

	UPROPERTY(EditDefaultsOnly, Category="Descensus|Inscription")
	TSubclassOf<ADesInscriptionCanvas> InscriptionCanvasClass;

	void CreateSlateWidgetAndAddToViewport(const TSharedRef<SWidget>& Widget, const int32 ZOrder) const;

public:
	UPROPERTY(BlueprintReadOnly)
	ECursorTarget CurrentCursorTarget = ECursorTarget::None;

	ADesHUD();
	virtual void Tick(float DeltaSeconds) override;

	void NewActorUnderCursor(const AActor* Actor, const UDesMetaComponent* MetaComponent);
	void UpdateActorUnderCursor(const AActor* Actor) const;

	void ShowTooltip(FDesTooltipData TooltipData, FVector2D DesiredPosition, bool bShouldAddVerticalOffset);
	void HideTooltip();

	FORCEINLINE UDesMainUILayer* GetMainUILayer() const { return MainUILayer; }

	FORCEINLINE ADesInscriptionCanvas* GetInscriptionCanvas() const { return InscriptionCanvas; }

	void InitForCharacter(const ADesPlayerCharacter* Character);

	void LookStarted();
	void LookCompleted();
};
