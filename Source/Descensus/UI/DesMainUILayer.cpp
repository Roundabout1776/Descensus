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

void UDesMainUILayer::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ItemLayer->UpdateEjectedItemPosition(SlateUser->GetCursorPosition(), InDeltaTime);
}

void UDesMainUILayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerController = MakeWeakObjectPtr(GetOwningPlayer<ADesPlayerController>());
	checkf(PlayerController.Get(), TEXT("MainUILayer couldn't be initialized without valid PlayerController!"))

	const auto LocalPlayer = PlayerController->GetLocalPlayer();

	SlateUser = LocalPlayer->GetSlateUser();

	const auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &ThisClass::HandleControlMappingsRebuilt);
}

void UDesMainUILayer::SetCrosshairVisible(const bool bNewVisible) const
{
	Crosshair->SetVisibility(bNewVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDesMainUILayer::HandleControlMappingsRebuilt()
{
	ShortcutsPanel->UpdateInputMappings(PlayerController.Get());
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
