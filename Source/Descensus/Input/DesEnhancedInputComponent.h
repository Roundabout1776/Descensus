#pragma once

#include "CoreMinimal.h"
#include "DesInputConfig.h"
#include "EnhancedInputComponent.h"
#include "DesEnhancedInputComponent.generated.h"

struct FGameplayTag;

UCLASS()
class DESCENSUS_API UDesEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename FuncType>
	void BindActionByTag(const UDesInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
	                     UserClass* Object, FuncType Func);


	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilities(const UDesInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                   ReleasedFuncType ReleasedFunc);
};

template <class UserClass, typename FuncType>
void UDesEnhancedInputComponent::BindActionByTag(const UDesInputConfig* InputConfig, const FGameplayTag& InputTag,
                                                 ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const auto* InputAction = InputConfig->FindInputActionForTag(InputTag))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UDesEnhancedInputComponent::BindAbilities(const UDesInputConfig* InputConfig, UserClass* Object,
                                               PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	for (const auto& AbilityInputAction : InputConfig->AbilityInputActions)
	{
		if (PressedFunc)
		{
			BindAction(AbilityInputAction.InputAction,
			           AbilityInputAction.bUseStartedTriggerEvent ? ETriggerEvent::Started : ETriggerEvent::Triggered,
			           Object, PressedFunc, static_cast<int32>(AbilityInputAction.AbilityInputID));
		}

		if (ReleasedFunc)
		{
			BindAction(AbilityInputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc,
			           static_cast<int32>(AbilityInputAction.AbilityInputID));
		}
	}
}
