#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_IceBeam.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_IceBeam : public UHASGameplayDamageAbility
{
	GENERATED_BODY()
	
public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel) override;

	UFUNCTION(BlueprintCallable)
	void BeamTrace(float BeamRadius, float BeamLength);

	UPROPERTY(EditDefaultsOnly)
	bool bDebugTrace = false;
};
