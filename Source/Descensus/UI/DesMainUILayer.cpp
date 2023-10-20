#include "UI/DesMainUILayer.h"

#include "EnhancedInputSubsystems.h"
#include "Components/DesItemContainerComponent.h"
#include "Components/Overlay.h"
#include "Player/DesPlayerController.h"
#include "Player/DesInventoryComponent.h"
#include "UI/DesShortcutsPanel.h"
#include "Materials/MaterialInstanceDynamic.h"

FReply UDesMainUILayer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if (ItemLayer->GetEjectedItem())
	// {
	// 	ItemLayer->InventoryComponent->ServerDestroyEjectedItem();
	// 	return FReply::Handled();
	// }
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDesMainUILayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerController = MakeWeakObjectPtr(GetOwningPlayer<ADesPlayerController>());
	checkf(PlayerController.Get(), TEXT("MainUILayer couldn't be initialized without valid PlayerController!"))

	const auto LocalPlayer = PlayerController->GetLocalPlayer();
	const auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &ThisClass::HandleControlMappingsRebuilt);
}

void UDesMainUILayer::HandleControlMappingsRebuilt()
{
	ShortcutsPanel->UpdateInputMappings(PlayerController.Get());
}
