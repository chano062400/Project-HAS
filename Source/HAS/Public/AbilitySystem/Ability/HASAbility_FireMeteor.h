#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_FireMeteor.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_FireMeteor : public UHASGameplayDamageAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SpawnMeteor(const FVector& TargetLocation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius;

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;
};
