#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_RushAttack.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_RushAttack : public UHASGameplayDamageAbility
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void Rush();

private:

	UFUNCTION()
	void OnMoveCompleted();

	UPROPERTY(EditDefaultsOnly)
	float TargetReachTime = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float KnockBackRadius = 500.f;
};
