#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HASAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AbilityActorInfoSet();

	void AddStartAbilities(TArray<TSubclassOf<UGameplayAbility>> StartAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
