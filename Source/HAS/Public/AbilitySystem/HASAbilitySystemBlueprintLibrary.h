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
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|GameplayEffectContext")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|GameplayEffectContext")
	static bool IsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);

};
