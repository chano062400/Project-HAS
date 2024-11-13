#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_ChargeScream.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_ChargeScream : public UHASGameplayDamageAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void KnockbackEnemiesWithinRadius();

private:
	
	UPROPERTY(EditDefaultsOnly)
	float KnockBackRadius = 750.f;
};
