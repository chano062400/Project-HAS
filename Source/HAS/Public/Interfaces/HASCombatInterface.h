#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "HASCombatInterface.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FMontageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

};

USTRUCT(BlueprintType)
struct FEnemyInfo
{
	FEnemyInfo() : Name(FName()), Level(int32()), HealthRatio(float()) {}

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(BlueprintReadOnly)
	int32 Level;

	UPROPERTY(BlueprintReadOnly)
	float HealthRatio;
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FMontageInfo GetMontageInfoByTag(const FGameplayTag& MontageTag); 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FEnemyInfo GetTargetInfo();

	virtual void Die() = 0;

};
