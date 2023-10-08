#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"

class FDesPreLoadScreen;

class FDesStartupLoadingScreenModule final : public IModuleInterface
{
	TSharedPtr<FDesPreLoadScreen> PreLoadScreen;

	static void ShutdownOnPreLoadScreenManagerCleanUp();

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool IsGameModule() const override;
};
