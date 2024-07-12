#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_FrostNova.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_FrostNova : public UHASGameplayDamageAbility
{
	GENERATED_BODY()

public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FrostRadius;

};
