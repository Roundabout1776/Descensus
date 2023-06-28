#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SDesPreLoadWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDesPreLoadWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
