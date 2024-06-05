#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "HASGameplayDamageAbility.generated.h"

class AHASProjectile;

/**
 * 
 */
UCLASS()
class HAS_API UHASGameplayDamageAbility : public UHASGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASProjectile> ProjectileClass;
};
