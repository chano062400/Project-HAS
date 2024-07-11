#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "GameplayTagContainer.h"
#include "HASAbilitySystemBlueprintLibrary.generated.h"

struct FHASDamageEffectParams;
struct FHASAbilityInfo;

/**
 * 
 */
UCLASS()
class HAS_API UHASAbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);
	
	UFUNCTION(BlueprintCallable)
	static bool IsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static bool IsApplyDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetIsApplyDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsApplyDebuff);
	
	UFUNCTION(BlueprintCallable)
	static FGameplayTag GetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable)
	static float GetDebuffChance(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetDebuffChance(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffChance);
	
	UFUNCTION(BlueprintCallable)
	static float GetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);
	
	UFUNCTION(BlueprintCallable)
	static float GetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);
	
	UFUNCTION(BlueprintCallable)
	static float GetDebuffDamge(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	
	UFUNCTION(BlueprintCallable)
	static float GetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InKnockbackForce);
	
	UFUNCTION(BlueprintCallable)
	static void GetActorsWithinRadius(UObject* WorldContextObject, TArray<AActor*>& OutOverlapingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure)
	static bool IsFriend(AActor* Actor1, AActor* Actor2);

	UFUNCTION(BlueprintPure)
	static bool IsDead(AActor* Actor);

	UFUNCTION(BlueprintPure)
	static FClassDefaultInfo GetClassDefaultInfo(UObject* WorldContextObejct, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure)
	static int32 GetLevelByXP(UObject* WorldContextObejct, int32 XP);

	UFUNCTION(BlueprintCallable)
	static void SpawnFireSphere(UObject* WorldContextObject, TArray<FVector>& OutVectors, float Radius, float Interval, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable)
	static FGameplayEffectContextHandle ApplyDamageEffectParams(const FHASDamageEffectParams& Params);

	UFUNCTION(BlueprintCallable)
	static FHASAbilityInfo FindAbilityInfoByTag(UObject* WorldContextObject, const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	static UAbilityInfo* GetAbilityInfo(UObject* WorldContextObject);

};
