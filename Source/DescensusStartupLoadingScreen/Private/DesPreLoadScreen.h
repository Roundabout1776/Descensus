#pragma once

#include "PreLoadScreenBase.h"
#include "PreLoadScreen.h"
#include "Templates/SharedPointer.h"
#include <atomic>

class FDesPreLoadScreen : public FPreLoadScreenBase
{
	TSharedPtr<SWidget> Widget = nullptr;
	double MinTime = 1.0;
	double InitTime = 0.0;
	std::atomic<bool> IsDoneFlag{false};

public:
	virtual void Init() override;
	virtual void Tick(float DeltaTime) override;
	virtual void CleanUp() override;
	virtual EPreLoadScreenTypes GetPreLoadScreenType() const override { return EPreLoadScreenTypes::EngineLoadingScreen; }
	virtual const TSharedPtr<const SWidget> GetWidget() const override { return Widget; }
	virtual TSharedPtr<SWidget> GetWidget() override { return Widget; }
	virtual bool IsDone() const override { return IsDoneFlag; }
};
