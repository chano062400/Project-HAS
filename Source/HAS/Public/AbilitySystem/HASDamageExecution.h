#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "HASDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UHASDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
