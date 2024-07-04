#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UHASGameplayDamageAbility;

USTRUCT(BlueprintType)
struct FHASAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag StatusTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 RequirementLevel;
	
	UPROPERTY(BlueprintReadWrite)
	int32 AbilityLevel;
	
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UHASGameplayDamageAbility> Ability;
};

/**
 * 
 */
UCLASS()
class HAS_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FHASAbilityInfo> AbilityInformation;

	FHASAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag);
};
