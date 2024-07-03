#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HASWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "SpellMenuWidgetController.generated.h"

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

};
