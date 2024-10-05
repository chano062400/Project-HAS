#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "HASAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityUpdateSignature, FGameplayAbilitySpec& /* AbilitySpec */);

DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityEquipSignature, FGameplayAbilitySpec& /* AbilitySpec */, const FGameplayTag /* PrevInputTag */);

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
	
	FAbilityEquipSignature AbilityEquipDelegate;
	
	bool bIsGivenStartAbilities = false;

	void AddStartAbilitiesByInputTag(TArray<TSubclassOf<UGameplayAbility>> StartAbilities);

	void AddHitReactAbility(TSubclassOf<UGameplayAbility> HitReactAbility);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	void ApplyDefaultAttributesByClass(ECharacterClass CharacterClass, int32 Level);

	void AddDefaultAbilitiesByClass(ECharacterClass CharacterClass, int32 Level);

	void AddCommonAbilities();

	void RemoveAllDebuffEffect();

	void ForEachAbility(const FForEachAbilitySignature& Delegate);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAbilityStatus(int32 Level);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(FGameplayAbilitySpec AbilitySpec);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAbility(const FGameplayTag& AbilityTag);
	
	UFUNCTION(Client, Reliable)
	void ClientUpgradeAbility(FGameplayAbilitySpec AbilitySpec);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAttribute(const FGameplayAttribute& Attribute, EGameplayModOp::Type ModOp, float Value);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAbilityInput(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityInput(FGameplayAbilitySpec InAbilitySpec, const FGameplayTag& PrevInputTag);

	virtual void OnRep_ActivateAbilities() override;

	FGameplayAbilitySpec* FindAbilitySpecByInputTag(const FGameplayTag& InputTag);

	FGameplayAbilitySpec* FindAbilitySpecByAbilityTag(const FGameplayTag& AbilityTag);
	
	FGameplayTag FindAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayTag FindStatusTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayTag FindInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	int32 FindPlayerLevelByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

	bool IsEquippedAbility(const FGameplayAbilitySpec& AbilitySpec);

	void ClearInput(FGameplayAbilitySpec* AbilitySpec);

	void UnEquipAbility(FGameplayAbilitySpec* AbilitySpec);
};

