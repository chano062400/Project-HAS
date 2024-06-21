#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HASWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedSignature, float , NewValue);

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

	void PlayerXPChanged(int32 NewXP);

	void PlayerLevelChanged(int32 NewLevel);

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature MaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature HealthChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature MaxManaChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature ManaChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature PlayerXPChangedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature PlayerLevelChangedDelegate;

};
