#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "HASAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

};
