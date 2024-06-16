#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "HASEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AHASAIController;

/**
 * 
 */
UCLASS()
class HAS_API AHASEnemy : public AHASCharacterBase
{
	GENERATED_BODY()

public:

	AHASEnemy();

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere)
	int32 Level = 1;

	virtual int32 GetLevel_Implementation() override { return Level; }

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature MaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature HealthChangedDelegate;

	virtual void Die() override;

	virtual void HitReactTagEvent(const FGameplayTag Tag, int32 NewCount) override;

	/* Combat Interface */

	virtual AActor* GetCombatTarget_Implementation() override;

	virtual	void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AHASAIController> HASAIController;

	UPROPERTY()
	AActor* CombatTarget = nullptr;
};
