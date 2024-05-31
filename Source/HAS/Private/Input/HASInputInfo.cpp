#include "Input/HASInputInfo.h"

const UInputAction* UHASInputInfo::FindAbilityInputActionByTag(const FGameplayTag& InputTag)
{
	for (auto InputAction : AbilityInputAction)
	{
		if (InputAction.InputTag.MatchesTagExact(InputTag))
		{
			return InputAction.InputAction;
		}
	}

	return nullptr;
}
