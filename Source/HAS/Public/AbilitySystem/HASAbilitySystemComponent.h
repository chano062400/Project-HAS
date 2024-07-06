#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "HASAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityUpdateSignature, FGameplayAbilitySpec& /* AbilitySpec */);

DECLARE_MULTICAST_DELEGATE(FStartAbilitiesGivenSignature);

DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);


/**
 * 
 */
UCLASS()
class HAS_API UHASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	FStartAbilitiesGivenSignature StartAbilitiesGivenDelegate;

	FAbilityUpdateSignature AbilityUpdateDelegate;

	FForEachAbilitySignature ForEachAbilityDelegate;

	bool bIsGivenStartAbilities = false;

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

	void ForEachAbility(const FForEachAbilitySignature& Delegate);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAbility(const FGameplayTag& AbilityTag);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbility(FGameplayAbilitySpec AbilitySpec);

	virtual void OnRep_ActivateAbilities() override;

	FGameplayAbilitySpec* FindAbilitySpecByTag(const FGameplayTag& AbilityTag);
	
	FGameplayTag FindAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayTag FindStatusTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	int32 FindPlayerLevelByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

};
