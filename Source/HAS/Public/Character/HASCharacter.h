#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "Interfaces/HASPlayerInterface.h"
#include "Inventory/HASInventoryComponent.h"
#include "HASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UNiagaraComponent;
class AHASMagicCircle;
class AHASItem;
class USceneCaptureComponent2D;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ApplyRegenerationEffect(TSubclassOf<UGameplayEffect> EffectClass);

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

	virtual AHASMagicCircle* ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;

	virtual void HideMagicCircle_Implementation() override;

	virtual void SetCastIceBeamLoop_Implementation(bool bInCastIcemBeamLoop) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDodging = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bCastIceBeamLoop = false;

	UFUNCTION(BlueprintCallable)
	UHASInventoryComponent* GetInventoryComponent() { return Inventory; }

	USceneCaptureComponent2D* GetSceneCaptureComponent2D() { return SceneCaptureComponent2D; }


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> LevelUpEffectComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> RegenerationEffectClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHASInventoryComponent> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent2D;
};
