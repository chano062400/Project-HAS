#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "HASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual FString GetAbilityDescription(int32 InAbilityLevel);

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};
