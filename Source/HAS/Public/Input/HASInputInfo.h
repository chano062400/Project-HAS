#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "HASInputInfo.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FHASInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

};

/**
 * 
 */
UCLASS()
class HAS_API UHASInputInfo : public UDataAsset
{

public:
	GENERATED_BODY()
	
	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FHASInputAction> AbilityInputAction;
};
