#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
};
