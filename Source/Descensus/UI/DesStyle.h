#pragma once

#include "CoreMinimal.h"
#include "Styling/ISlateStyle.h"

class FDesStyle
{
protected:
	UPROPERTY()
	static TSharedPtr<FSlateStyleSet> Instance;

public:
	static const ISlateStyle& Get();
	static void Initialize();
	static void Shutdown();
};
