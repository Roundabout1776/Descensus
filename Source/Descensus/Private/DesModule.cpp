#include "DesModule.h"

#include "Modules/ModuleManager.h"
#include "UI/DesStyle.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDesModule, Descensus, "Descensus");

FDesStyle* FDesModule::StyleInstance = nullptr;

void FDesModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	StyleInstance = new FDesStyle();
	StyleInstance->Initialize();
}

void FDesModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
	StyleInstance->Shutdown();
	delete StyleInstance;
}
