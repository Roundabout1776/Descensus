#include "UI/DesStyle.h"

#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FDesStyle::Instance = nullptr;

static const FName StyleName(TEXT("DesStyle"));

const ISlateStyle& FDesStyle::Get()
{
	return *Instance;
}

void FDesStyle::Initialize()
{
	if (!Instance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(StyleName);

		Instance = FSlateGameResources::New(StyleName, "/Game/BaseGame/UI",
		                                               "/Game/BaseGame/UI");
		// DES_LOG(Warning, TEXT("%i"), Instance.Get()->GetBrush("Brush/SB_CommonBox"));
		FSlateStyleRegistry::RegisterSlateStyle(*Instance);
	}
}

void FDesStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleName);
	if (Instance.IsValid())
	{
		Instance.Reset();
	}
}
