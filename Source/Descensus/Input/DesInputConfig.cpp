#include "Input/DesInputConfig.h"

const UInputAction* UDesInputConfig::FindInputActionForTag(const FGameplayTag& InInputTag) const
{
	for (const auto& [InputAction, InputTag] : TaggedInputActions)
	{
		if (InputAction && InputTag == InInputTag)
		{
			return InputAction;
		}
	}

	return nullptr;
}
