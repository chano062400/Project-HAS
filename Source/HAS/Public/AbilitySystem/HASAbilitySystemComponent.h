#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "HASAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityUpdateSignature, FGameplayAbilitySpec& /* AbilitySpec */, bool /* bIsStartAbility */);

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

	void AddDefaultAbilitiesByClass(ECharacterClass CharacterClass, int32 Level);

	void AddCommonAbilities();

	void RemoveAllDebuffEffect();

	void AbilityLevelUp(const FGameplayTag& AbilityTag);

	FGameplayAbilitySpec* FindAbilitySpecByTag(const FGameplayTag& AbilityTag);
	
	FGameplayTag FindAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayTag FindStatusTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	int32 FindPlayerLevelByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	FAbilityUpdateSignature AbilityUpdateDelegate;

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
