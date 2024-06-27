#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_FireMeteor.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_FireMeteor : public UHASGameplayDamageAbility
{
	GENERATED_BODY()
	
	virtual void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming) override;
};
