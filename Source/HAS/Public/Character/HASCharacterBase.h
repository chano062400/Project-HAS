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
class UNiagaraComponent;

UCLASS()
class HAS_API AHASCharacterBase : public ACharacter, public IAbilitySystemInterface, public IHASCombatInterface
{
	GENERATED_BODY()

public:

	AHASCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSetComp; }

	virtual void InitializeDefaultAttributesByClass(ECharacterClass InCharacterClass, int32 Level);

	virtual void AddDefaultAbilitiesByClass(ECharacterClass InCharacterClass, int32 Level);

	virtual void InitializeDefaultAttributes();

	virtual void ApplyAttribute(TSubclassOf<UGameplayEffect> Attribute, AActor* SourceObject);

	virtual void InitAbilityActorInfo();

	virtual	void AddStartAbilities();

	virtual void AddCommonAbilities();

	virtual void HitReactTagEvent(const FGameplayTag Tag, int32 NewCount);

	virtual void DebuffTagEvent(const FGameplayTag Tag, int32 NewCount);

	/* Combat Interface */

	virtual int32 GetLevel_Implementation() override;

	virtual void HighlightActor() override;

	virtual void UnHighlightActor() override;

	virtual FVector GetWeaponSocketLocation_Implementation(const FGameplayTag& SocketTag) override;

	virtual void Die() override;

	virtual FMontageInfo GetMontageInfoByTag_Implementation(const FGameplayTag& MontageTag) override;

	virtual bool IsDead_Implementation() const override;


	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDie();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDieEffect(int32 MaterialIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastFreezeDebuffHandle(bool bIsFrozen);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFreezeEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void ReturnToDefaultMaterial();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed = 250.f;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FMontageInfo> MontageInformations;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayAbility> HitReactAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DieEffectMI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DieEffectMaterialIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> FreezeEffectMI;

	UPROPERTY(EditAnywhere, Category = "Class")
	ECharacterClass CharacterClass = ECharacterClass::ECC_None;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultVitalAttribute;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultPrimrayAttribute;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttribute;

	bool bDead = false;

	TMap<FGameplayTag, UNiagaraComponent*> DebuffTagToNiagara;

	/* Debuff Niagara Component */

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> BurnDebuffComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> ElectricShockDebuffComponent;
	
};
