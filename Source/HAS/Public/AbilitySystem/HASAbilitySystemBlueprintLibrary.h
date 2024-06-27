#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "HASAbilitySystemBlueprintLibrary.generated.h"

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
	static void GetPlayersWithinRadius(UObject* WorldContextObject, TArray<AActor*>& OutOverlapingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

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

};
