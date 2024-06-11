#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "HASCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS()
class HAS_API AHASCharacterBase : public ACharacter, public IAbilitySystemInterface, public IHASCombatInterface
{
	GENERATED_BODY()

public:

	AHASCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSetComp; }

	virtual void InitializeDefaultAttributesByClass(ECharacterClass InCharacterClass, int32 Level);

	virtual void InitAbilityActorInfo();

	virtual	void AddStartAbilities();

	virtual void AddHitReactAbility(TSubclassOf<UGameplayAbility> InHitReactAbility);

	/* Combat Interface */

	virtual int32 GetLevel_Implementation() override;

	virtual void HighlightActor() override;

	virtual void UnHighlightActor() override;

	virtual FVector GetWeaponSocketLocation_Implementation(const FGameplayTag& SocketTag) override;

	virtual void HitReactTagEvent(const FGameplayTag Tag, int32 NewCount);

	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDie();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDieEffect(int32 MaterialIndex);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual FMontageInfo GetMontageInfo_Implementation(const FGameplayTag& MontageTag);

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<FMontageInfo> MontageInformations;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed = 250.f;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSetComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> HitReactAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DieEffectMI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DieEffectMaterialIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	ECharacterClass CharacterClass = ECharacterClass::ECC_None;
};
