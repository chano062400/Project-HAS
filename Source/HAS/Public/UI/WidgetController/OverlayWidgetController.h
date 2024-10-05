#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HASWidgetController.h"
#include "Interfaces/HASCombatInterface.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedSignature, float , NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerXPPercentChangedSignature, float , NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerLevelChangedSignature, int32, NewValue, bool, bLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTargetInfoSignature, FEnemyInfo, Info, int32, PlayerLevel);

/**
 * 
 */
UCLASS()
class HAS_API UOverlayWidgetController : public UHASWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;

	virtual void BindCallBacks() override;

	UFUNCTION(BlueprintCallable)
	void OnDropped(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature MaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature HealthChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature MaxManaChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature ManaChanged;

	UPROPERTY(BlueprintAssignable)
	FPlayerXPPercentChangedSignature PlayerXPPercentChangedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerLevelChangedSignature PlayerLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FTargetInfoSignature EnemyInfoDelegate;
};
