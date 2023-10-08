#include "SDesPreLoadWidget.h"

#include "SlateOptMacros.h"
#include "CoreMinimal.h"
#include "Widgets/Images/SThrobber.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDesPreLoadWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
	    .VAlign(VAlign_Center)
	    .HAlign(HAlign_Center)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor::Black)
		.Padding(0)
		[
			SNew(SCircularThrobber)
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
