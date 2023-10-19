#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SDesCrosshair;

class DESCENSUS_API SDesHUDLayer final : public SCompoundWidget
{
protected:
	TSharedPtr<SDesCrosshair> Crosshair;
	
public:
	SLATE_BEGIN_ARGS(SDesHUDLayer)
		{
		}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	void SetCrosshairVisible(bool bNewVisible) const;
};
