#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "HASEnemy.generated.h"

class UWidgetComponent;
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

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBarWidget;

};
