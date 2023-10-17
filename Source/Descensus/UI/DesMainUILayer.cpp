#include "UI/DesMainUILayer.h"

#include "DesLogging.h"
#include "EnhancedInputSubsystems.h"
#include "Components/DesInventoryComponent.h"
#include "Components/DesItemContainerComponent.h"
#include "Components/Overlay.h"
#include "Items/DesItemContainerWidget.h"
#include "Items/DesItemLayer.h"
#include "Player/DesPlayerController.h"
#include "UI/DesShortcutsPanel.h"
#include "Materials/MaterialInstanceDynamic.h"

FReply UDesMainUILayer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!ItemLayer->IsLocked() && ItemLayer->GetEjectedItem())
	{
		SetCursor(EMouseCursor::None);
		ItemLayer->Lock();
		ItemLayer->InventoryComponent->ServerDestroyEjectedItem();
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDesMainUILayer::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	PolledPointerEvent = InMouseEvent;
	return FReply::Unhandled();
}

void UDesMainUILayer::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ItemLayer->HandlePointer(PolledPointerEvent, InDeltaTime);
}

void UDesMainUILayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto PlayerController = GetOwningPlayer<ADesPlayerController>();
	checkf(PlayerController, TEXT("MainUILayer couldn't be initialized without valid PlayerController!"))

	const auto LocalPlayer = PlayerController->GetLocalPlayer();
	const auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	InputSystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &ThisClass::HandleControlMappingsRebuilt);
}

void UDesMainUILayer::SetCrosshairVisible(const bool bNewVisible) const
{
	Crosshair->SetVisibility(bNewVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDesMainUILayer::HandleControlMappingsRebuilt()
{
	ShortcutsPanel->UpdateInputMappings(GetOwningPlayer<ADesPlayerController>());
}

void UDesMainUILayer::SetupItemSystem(UDesInventoryComponent* InventoryComponent)
{
	Inventory->AttachToItemContainerComponent(Cast<UDesItemContainerComponent>(InventoryComponent));
	Inventory->SetItemLayer(ItemLayer);

	ItemLayer->AttachToInventory(InventoryComponent);

	/* @TODO: force update cursor. */
	InventoryComponent->OnEjectedItemChanged.AddWeakLambda(this, [this](const UDesItemInstance* EjectedItem)
	{
		if (EjectedItem)
		{
			SetCursor(EMouseCursor::None);
		}
		else
		{
			SetCursor(EMouseCursor::Default);
		}
	});
}
