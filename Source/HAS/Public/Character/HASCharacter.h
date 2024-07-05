#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "Interfaces/HASPlayerInterface.h"
#include "HASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class HAS_API AHASCharacter : public AHASCharacterBase, public IHASPlayerInterface
{
	GENERATED_BODY()
	
public:
	
	AHASCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayLevelUpEffect();

	/* Combat Interface */
	
	virtual int32 GetLevel_Implementation() override;

	/* Player Interface */

	virtual void SetXP(int32 NewXP) override;

	virtual void SetLevel(int32 NewLevel) override;

	virtual int32 GetXP() override;

	virtual int32 GetAttributePoint() override;

	virtual void SetAttributePoint(int32 NewAttributePoint) override;

	virtual int32 GetSpellPoint() override;

	virtual void SetSpellPoint(int32 NewSpellPoint) override;

	virtual UAttributeSet* GetAttributeSet() override;

	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;

	virtual void HideMagicCircle_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDodging = false;

protected:

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> LevelUpEffectComponent;

};
