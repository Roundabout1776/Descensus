#pragma once

#include "DesAbilityInputID.generated.h"

UENUM(BlueprintType)
enum class EDesAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	DrawWeapon,
	Inscribe,
	Run,
	Jump
};
