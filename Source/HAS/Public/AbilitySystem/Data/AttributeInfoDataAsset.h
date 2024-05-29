#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfoDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FHASAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue;

};

/**
 * 
 */
UCLASS()
class HAS_API UAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	FHASAttributeInfo FindAttributeInfoByTag(const FGameplayTag& AttributeTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FHASAttributeInfo> AttributeInformation;

};
