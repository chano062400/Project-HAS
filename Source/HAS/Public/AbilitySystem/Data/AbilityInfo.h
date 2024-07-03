#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FHASAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName AbilityName;

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
