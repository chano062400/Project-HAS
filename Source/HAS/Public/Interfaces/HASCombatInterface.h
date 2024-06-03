#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HASCombatInterface.generated.h"

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

};
