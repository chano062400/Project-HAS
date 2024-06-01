#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/HASInputInfo.h"
#include "HASEnhancedInputComponent.generated.h"



/**
 * 
 */
UCLASS()
class HAS_API UHASEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityAction(const UHASInputInfo* InputConfig, UserClass* Object, PressedFuncType PressFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UHASEnhancedInputComponent::BindAbilityAction(const UHASInputInfo* InputInfo, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputInfo);

	for (const FHASInputAction& Action : InputInfo->AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// Callback함수와 매개변수(InputTag) 설정.

			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				// Trigger - 입력이 유지되는 동안 프레임마다 호출.
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
