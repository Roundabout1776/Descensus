#include "SDesShortcutSlot.h"

#include "EnhancedInputSubsystems.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Engine/Texture2D.h"
#include "UI/DesStyle.h"
#include "InputAction.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesShortcutSlot::Construct(const FArguments& InArgs)
{
	InputAction = MakeWeakObjectPtr(InArgs._InputAction.Get());

	const auto SizeOverride = InArgs._SizeOverride.Get();

	const auto Style = FDesStyle::GetDefaultStyle();

	ChildSlot.SetPadding(FMargin(0.0f, 0.0f, 7.0f, 0.0f));
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SBox)
			.HeightOverride(SizeOverride)
			.WidthOverride(SizeOverride)
			[
				SNew(SBorder)
				.BorderImage(&Style->CommonBox)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SAssignNew(IconImage, SImage)
						.Image(&IconSlateBrush)
					]
					+ SOverlay::Slot()
					  .VAlign(VAlign_Bottom)
					  .HAlign(HAlign_Right)
					  .Padding(0.0f, 0.0f, 11.0f, 5.5f)
					[
						SAssignNew(InputTextBlock, STextBlock)
						.Justification(ETextJustify::Right)
						.TextStyle(&Style->DefaultTextStyle)
						.Text(INVTEXT("F"))
					]
				]
			]
		]
	];

	SetImageFromTexture(InArgs._FixedIconTexture.Get());
}

void SDesShortcutSlot::SetImageFromTexture(UTexture2D* Texture)
{
	if (IsValid(Texture))
	{
		IconSlateBrush.SetResourceObject(Texture);
		IconSlateBrush.DrawAs = ESlateBrushDrawType::Image;
	}
	else
	{
		IconSlateBrush.SetResourceObject(nullptr);
		IconSlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	}
}

void SDesShortcutSlot::UpdateInputText(const UEnhancedInputLocalPlayerSubsystem* InputSystem) const
{
	if (InputTextBlock.IsValid())
	{
		const auto Keys = InputSystem->QueryKeysMappedToAction(InputAction.Get());
		if (Keys.IsEmpty())
		{
			InputTextBlock.Get()->SetText(FText::GetEmpty());
		}
		else
		{
			const auto& Key = Keys[0];
			InputTextBlock.Get()->SetText(Key.GetDisplayName());
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
