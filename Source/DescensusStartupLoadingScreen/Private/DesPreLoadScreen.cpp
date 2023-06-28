#include "DesPreLoadScreen.h"

#include "SDesPreLoadWidget.h"

void FDesPreLoadScreen::Init()
{
	if (FApp::CanEverRender())
	{
		Widget = SNew(SDesPreLoadWidget);
	}

	InitTime = FPlatformTime::Seconds();
}

void FDesPreLoadScreen::Tick(float DeltaTime)
{
	FPreLoadScreenBase::Tick(DeltaTime);
	if (IsEngineExitRequested() || (FPlatformTime::Seconds() - InitTime > MinTime))
	{
		IsDoneFlag = true;
	}
}

void FDesPreLoadScreen::CleanUp()
{
	FPreLoadScreenBase::CleanUp();
	Widget.Reset();
}
