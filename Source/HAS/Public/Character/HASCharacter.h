#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "HASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

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
	
	virtual void InitAbilityActorInfo() override;

	/* Combat Interface */
	
	virtual int32 GetLevel_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDodging = false;
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGroomComponent> Hair;


};
