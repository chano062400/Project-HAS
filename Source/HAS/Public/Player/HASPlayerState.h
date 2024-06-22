#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "HASPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelXPInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStatChangedSignature, int32);

/**
 * 
 */
UCLASS()
class HAS_API AHASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AHASPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UAttributeSet* GetAttributeSet() const;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	/* Level */

	FPlayerStatChangedSignature PlayerLevelChangedDelegate;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	void SetLevel(int32 NewLevel);

	FORCEINLINE int32 GetLevel() { return Level; }

	/* XP */

	FPlayerStatChangedSignature PlayerXPChangedDelegate;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0.f;

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	void SetXP(int32 NewXP);

	FORCEINLINE int32 GetXP() { return XP; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelXPInfo> LevelXPInformation;

private:

};
