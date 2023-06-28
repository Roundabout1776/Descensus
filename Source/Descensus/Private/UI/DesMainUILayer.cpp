#include "UI/DesMainUILayer.h"

#include "DesLogging.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Player/DesPlayerController.h"
#include "UI/DesShortcutsPanel.h"

void UDesMainUILayer::HandleControlMappingsRebuilt()
{
	ShortcutsPanel->UpdateInputMappings(GetOwningPlayer<ADesPlayerController>());
}

void UDesMainUILayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (const auto PlayerController = GetOwningPlayer<ADesPlayerController>())
	{
		const auto LocalPlayer = PlayerController->GetLocalPlayer();
		const auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		InputSystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &ThisClass::HandleControlMappingsRebuilt);
	}
}

void UDesMainUILayer::SetInscriptionOverlayMaterial(UMaterialInstanceDynamic* Material) const
{
	InscriptionOverlay->SetBrushFromMaterial(Material);
}

void UDesMainUILayer::SetCrosshairVisible(const bool bNewVisible) const
{
	Crosshair->SetVisibility(bNewVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
