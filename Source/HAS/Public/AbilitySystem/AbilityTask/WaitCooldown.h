#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "WaitCooldown.generated.h"

class UAbilitysystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownSignature, float, CooldownRemaining);

/**
 * 
 */

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "Async Task"))
class HAS_API UWaitCooldown : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldown* WaitForCooldown(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag);

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnCooldownAppliedToSelf(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& EffectSpec , FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UPROPERTY(BlueprintAssignable)
	FCooldownSignature CooldownStart;
	
	UPROPERTY(BlueprintAssignable)
	FCooldownSignature CooldownEnd;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

};
