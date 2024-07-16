#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "HASAbility_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_Teleport : public UHASGameplayAbility
{
	GENERATED_BODY()

public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;
};
