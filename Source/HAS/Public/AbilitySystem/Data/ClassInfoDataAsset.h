#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "ClassInfoDataAsset.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	ECC_None,
	ECC_Archer,
	ECC_Warrior,
	ECC_Hound,
	ECC_Boss
};

USTRUCT(BlueprintType)
struct FClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Default Information")
	TSubclassOf<UGameplayEffect> PrimaryAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Class Default Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Class Default LevelXP")
	FScalableFloat XPReward;
};

/**
 * 
 */
UCLASS()
class HAS_API UClassInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category ="Character Class Informations")
	TMap<ECharacterClass, FClassDefaultInfo> CharacterClassInformations;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Information")
	TSubclassOf<UGameplayEffect> SecondaryAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Information")
	TSubclassOf<UGameplayEffect> VitalAttribute;

	UPROPERTY(EditDefaultsOnly, Category= "Common Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FClassDefaultInfo GetCharacterClassInfo(ECharacterClass CharacterClass);
};
