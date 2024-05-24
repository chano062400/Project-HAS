#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "HASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class HAS_API AHASCharacter : public AHASCharacterBase
{
	GENERATED_BODY()
	
public:
	
	AHASCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	virtual void InitializeStartAttributes() override;

	virtual void InitAbilityActorInfo() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;


};
