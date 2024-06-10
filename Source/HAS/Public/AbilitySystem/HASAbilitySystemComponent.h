#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
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

	void AddStartAbilitiesByInputTag(TArray<TSubclassOf<UGameplayAbility>> StartAbilities);

	void AddHitReactAbility(TSubclassOf<UGameplayAbility> HitReactAbility);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	void ApplyDefaultAttributesByClass(ECharacterClass CharacterClass, int32 Level);

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
