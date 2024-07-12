#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_LightningShield.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_LightningShield : public UHASGameplayDamageAbility
{
	GENERATED_BODY()

public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;
};
