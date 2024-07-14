#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_IceTornado.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_IceTornado : public UHASGameplayDamageAbility
{
	GENERATED_BODY()

public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;
};
