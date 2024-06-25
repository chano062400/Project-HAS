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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;

	/* Level */

	FPlayerStatChangedSignature PlayerLevelChangedDelegate;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	void SetLevel(int32 NewLevel);

	FORCEINLINE int32 GetLevel() { return Level; }

	/* XP */

	FPlayerStatChangedSignature PlayerXPChangedDelegate;

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	void SetXP(int32 NewXP);

	FORCEINLINE int32 GetXP() { return XP; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelXPInfo> LevelXPInformation;

	/* Attribute Point */

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

	void SetAttributePoint(int32 NewAttributePoint);

	FORCEINLINE int32 GetAttributePoint() { return AttributePoint; }

	FPlayerStatChangedSignature PlayerAttributePointChangedDelegate;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0.f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint)
	int32 AttributePoint = 0;

};
