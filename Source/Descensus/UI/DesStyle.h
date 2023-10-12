#pragma once

#include "CoreMinimal.h"
#include "DesWidgetStyleCatalog.h"
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

	static const FDesSlateWidgetStyle* GetDefaultStyle()
	{
		return &Get().GetWidgetStyle<FDesSlateWidgetStyle>("SWS_Descensus");
	}
};
