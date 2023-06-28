#include "DesStartupLoadingScreenModule.h"

#include "DesPreLoadScreen.h"
#include "PreLoadScreenManager.h"

IMPLEMENT_MODULE(FDesStartupLoadingScreenModule, DescensusStartupLoadingScreen)

void FDesStartupLoadingScreenModule::StartupModule()
{
	if (IsRunningDedicatedServer() || !FApp::CanEverRender() || !FPreLoadScreenManager::Get())
	{
		return;
	}

	PreLoadScreen = MakeShared<FDesPreLoadScreen>();
	if (PreLoadScreen.IsValid())
	{
		FPreLoadScreenManager::Get()->RegisterPreLoadScreen(PreLoadScreen);
		FPreLoadScreenManager::Get()->OnPreLoadScreenManagerCleanUp.AddStatic(&FDesStartupLoadingScreenModule::ShutdownOnPreLoadScreenManagerCleanUp);
		PreLoadScreen->Init();
	}
}

void FDesStartupLoadingScreenModule::ShutdownModule()
{
	if (PreLoadScreen.IsValid())
	{
		FPreLoadScreenManager::Get()->UnRegisterPreLoadScreen(PreLoadScreen);
	}
	PreLoadScreen.Reset();
}

bool FDesStartupLoadingScreenModule::IsGameModule() const
{
	return true;
}

void FDesStartupLoadingScreenModule::ShutdownOnPreLoadScreenManagerCleanUp()
{
	FModuleManager::Get().UnloadModule("DescensusStartupLoadingScreen", false);
}
