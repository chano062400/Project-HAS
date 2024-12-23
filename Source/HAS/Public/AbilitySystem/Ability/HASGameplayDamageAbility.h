#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "HASGameplayDamageAbility.generated.h"

class AHASProjectile;
struct FHASDamageEffectParams;

/**
 * 
 */
UCLASS()
class HAS_API UHASGameplayDamageAbility : public UHASGameplayAbility
{
	GENERATED_BODY()

public:

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming);

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(AActor* CombatTarget, int32 Level);

	UFUNCTION(BlueprintPure)
	FHASDamageEffectParams MakeDamageEffectParams(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AHASProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	float DebuffChance;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	float DebuffDuration;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	float DebuffFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	float DebuffDamage;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	float KnockbackForceMagnitude;

	UPROPERTY(EditDefaultsOnly, Category = "DamageParams")
	float KnockbackChance;

};
