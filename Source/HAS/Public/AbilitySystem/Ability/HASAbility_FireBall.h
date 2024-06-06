#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_FireBall.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_FireBall : public UHASGameplayDamageAbility
{
	GENERATED_BODY()
	
public:
	
	virtual void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming) override;
};
