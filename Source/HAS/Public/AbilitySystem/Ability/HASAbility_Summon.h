#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "HASAbility_Summon.generated.h"

class AHASEnemy;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_Summon : public UHASGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void Summon();

private:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AHASEnemy>> SummonActorClasses;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> SummonEffect;

	UPROPERTY(EditDefaultsOnly)
	float Range = 180.f;

	UPROPERTY(EditDefaultsOnly)
	float Interval = 45.f;
	
	UPROPERTY(EditDefaultsOnly)
	float Length = 150.f;

};
