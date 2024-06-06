#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HASAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayEffectModCallbackData;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	UPROPERTY(EditAnywhere)
	FGameplayEffectContextHandle EffectContextHandle;

	//Source

	UPROPERTY(EditAnywhere)
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY(EditAnywhere)
	AController* SourceController = nullptr;

	UPROPERTY(EditAnywhere)
	ACharacter* SourceCharacter = nullptr;

	//Target

	UPROPERTY(EditAnywhere)
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY(EditAnywhere)
	AController* TargetController = nullptr;

	UPROPERTY(EditAnywhere)
	ACharacter* TargetCharacter = nullptr;

};

/**
 * 
 */
UCLASS()
class HAS_API UHASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UHASAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void SetEffectProps(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps);

	void HandleIncomingDamage(FEffectProperties& Props);

	/* Vital Attributes */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, Health);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	/* Primary Attributes */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, Vigor);

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, Intelligence);

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OIdIntelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, Dexterity);

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OIdDexterity);

	/* Secondary Attributes */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, CriticalChance);

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OIdCriticalChacne);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalResistance;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, CriticalResistance);

	UFUNCTION()
	void OnRep_CriticalResistance(const FGameplayAttributeData& OIdCriticalResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, MaxHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	
	// Meta Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData InComingDamage;
	ATTRIBUTE_ACCESSORS(UHASAttributeSet, InComingDamage);

	UPROPERTY()
	TMap<FGameplayTag, FGameplayAttribute> TagsToAttributes;
};
