#pragma once

class FDesStyle;

class FDesModule final : public FDefaultGameModuleImpl
{
protected:
	static FDesStyle* StyleInstance;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


