#include "UI/DesShortcutsPanel.h"

#include "DesLogging.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "Player/DesPlayerController.h"
#include "UI/DesStyle.h"
#include "UI/SDesShortcutSlot.h"

TSharedRef<SWidget> UDesShortcutsPanel::RebuildWidget()
{
	SAssignNew(ShortcutsContainer, SHorizontalBox);

	Shortcuts.Reserve(SlotDataArray.Num());
	for (const auto& SlotData : SlotDataArray)
	{
		const TSharedPtr<SDesShortcutSlot> ShortcutSlot = SNew(SDesShortcutSlot)
		.InputAction(SlotData.InputAction)
		.SizeOverride(SlotData.Size)
		.FixedIconTexture(SlotData.bFixed ? SlotData.FixedIconTexture : nullptr);

		ShortcutsContainer->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Bottom)
		[
			ShortcutSlot.ToSharedRef()
		];
		
		Shortcuts.Emplace(ShortcutSlot);
	}

	return ShortcutsContainer.ToSharedRef();
}

void UDesShortcutsPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UDesShortcutsPanel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Shortcuts.Reset();
	ShortcutsContainer.Reset();
}

void UDesShortcutsPanel::UpdateInputMappings(const ADesPlayerController* PlayerController)
{
	const auto LocalPlayer = PlayerController->GetLocalPlayer();
	const auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	for (const auto Shortcut : Shortcuts)
	{
		Shortcut->UpdateInputText(InputSystem);
	}
}
