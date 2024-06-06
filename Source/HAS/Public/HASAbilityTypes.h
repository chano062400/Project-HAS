#pragma once
#include "GameplayEffectTypes.h"
#include "HASAbilityTypes.generated.h"

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

	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

protected:

	UPROPERTY()
	bool bIsCriticalHit = false;



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
