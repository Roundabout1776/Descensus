#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDesRuneDataActions;

class FDesEditorModule : public IModuleInterface
{
	TSharedPtr<FDesRuneDataActions> RuneDataActions;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
