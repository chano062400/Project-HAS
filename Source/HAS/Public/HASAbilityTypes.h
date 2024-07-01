#pragma once
#include "GameplayEffectTypes.h"
#include "HASAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FHASDamageEffectParams
{
	GENERATED_BODY()

	FHASDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObejct = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

};

USTRUCT(BlueprintType)
struct FHASGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	// 필수 override 함수

	// Returns the actual struct used for serialization, subclasses must override this!
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	// Custom serialization, subclasses must override this
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FHASGameplayEffectContext* Duplicate() const
	{
		FHASGameplayEffectContext* NewContext = new FHASGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// Critical Hit

	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetCriticalHit(bool InIsCriticalHit) { bIsCriticalHit = InIsCriticalHit; }

	// Debuff

	TSharedPtr<FGameplayTag> GetDamageType() { return DamageType; }

	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }

	bool IsApplyDebuff() const { return bIsApplyDebuff; }

	void SetIsApplyDebuff(bool bInIsApplyDebuff) { bIsApplyDebuff = bInIsApplyDebuff; }

	float GetDebuffChance() { return DebuffChance; }

	void SetDebuffChance(float InDebuffChance) { DebuffChance = InDebuffChance; }

	float GetDebuffDuration() { return DebuffDuration; }

	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }

	float GetDebuffFrequency() { return DebuffFrequency; }

	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }

	float GetDebuffDamage() { return DebuffDamage; }

	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }

protected:

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsApplyDebuff = false;

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	TSharedPtr<FGameplayTag> DamageType;
};	

template<>
struct TStructOpsTypeTraits<FHASGameplayEffectContext > : public TStructOpsTypeTraitsBase2<FHASGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, // struct has a NetSerialize function for serializing its state to an FArchive used for network replication.
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
