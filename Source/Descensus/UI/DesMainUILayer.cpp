#include "UI/DesMainUILayer.h"

#include "DesLogging.h"
#include "EnhancedInputSubsystems.h"
#include "Components/DesItemContainerComponent.h"
#include "Components/Overlay.h"
#include "Items/DesItemContainerWidget.h"
#include "Items/DesItemLayer.h"
#include "Player/DesPlayerController.h"
#include "UI/DesShortcutsPanel.h"
#include "Materials/MaterialInstanceDynamic.h"

FReply UDesMainUILayer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemLayer->IsItemMoveActive())
	{
		ItemLayer->GetContainerToMoveFrom()->ServerDestroyItem(ItemLayer->GetItemToMove());
		ItemLayer->EndItemMove();
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDesMainUILayer::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ItemLayer->HandlePointer(InMouseEvent);
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
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

	Inventory->SetItemLayer(ItemLayer);
}

void UDesMainUILayer::SetCrosshairVisible(const bool bNewVisible) const
{
	Crosshair->SetVisibility(bNewVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDesMainUILayer::HandleControlMappingsRebuilt()
{
	ShortcutsPanel->UpdateInputMappings(GetOwningPlayer<ADesPlayerController>());
}
