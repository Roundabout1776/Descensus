#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DesStatusBar.generated.h"

class UCreatureModel;
class UDesStatusBarResource;
struct FOnAttributeChangeData;

UCLASS(Abstract)
class DESCENSUS_API UDesStatusBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnAttributeChangedHandler(const FOnAttributeChangeData& Data);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesStatusBarResource> HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesStatusBarResource> ManaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDesStatusBarResource> StaminaBar;
};
