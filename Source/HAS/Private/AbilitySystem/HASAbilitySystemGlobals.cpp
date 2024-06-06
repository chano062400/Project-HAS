#include "AbilitySystem/HASAbilitySystemGlobals.h"
#include "HASAbilityTypes.h"

FGameplayEffectContext* UHASAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FHASGameplayEffectContext();
}
