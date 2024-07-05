// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "HASAbility_FireWall.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAbility_FireWall : public UHASGameplayDamageAbility
{
	GENERATED_BODY()
	
public:

	virtual FString GetAbilityDescription(int32 InAbilityLevel);
};
