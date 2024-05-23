#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "HASCharacter.generated.h"

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

private:

	void InitAbilityActorInfo();
};
