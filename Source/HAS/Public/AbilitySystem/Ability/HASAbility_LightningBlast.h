#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_LightningBlast.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_LightningBlast : public UHASGameplayDamageAbility
{
	GENERATED_BODY()

public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;

	UFUNCTION(BlueprintCallable)
	void LightningBlast();

	UPROPERTY(EditDefaultsOnly)
	float BlastRadius;

};
