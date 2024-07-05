#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HASWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellPointChangedSignature, int32, NewSpellPoint);

/**
 * 
 */
UCLASS()
class HAS_API USpellMenuWidgetController : public UHASWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;

	virtual void BindCallBacks() override;

	UFUNCTION(BlueprintCallable)
	void SpellBoxPressed(const FGameplayTag& AbilityTag);

	UPROPERTY(BlueprintAssignable)
	FSpellPointChangedSignature SpellPointChangedDelegate;
};
