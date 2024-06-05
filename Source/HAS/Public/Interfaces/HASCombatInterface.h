#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "HASCombatInterface.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FAttackMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

};

UINTERFACE(MinimalAPI)
class UHASCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HAS_API IHASCombatInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetLevel();

	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0 ;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMotionWarpingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetWeaponSocketLocation(const FGameplayTag& SocketTag);

};
