#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class SImage;
class STextBlock;
class UTexture2D;

class DESCENSUS_API SDesShortcutSlot : public SCompoundWidget
{
	FSlateBrush IconSlateBrush;
	TSharedPtr<SImage> IconImage;
	TSharedPtr<STextBlock> InputTextBlock;
	TWeakObjectPtr<UInputAction> InputAction;
	
public:
	SLATE_BEGIN_ARGS(SDesShortcutSlot)
		{
		}

	SLATE_ATTRIBUTE(TObjectPtr<UTexture2D>, FixedIconTexture)
	SLATE_ATTRIBUTE(TObjectPtr<UInputAction>, InputAction)
	SLATE_ATTRIBUTE(float, SizeOverride)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetImageFromTexture(UTexture2D* Texture);
	void UpdateInputText(const UEnhancedInputLocalPlayerSubsystem* InputSystem) const;
};
