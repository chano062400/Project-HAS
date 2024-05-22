#pragma once

#include "CoreMinimal.h"
#include "Character/HASCharacterBase.h"
#include "HASEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API AHASEnemy : public AHASCharacterBase
{
	GENERATED_BODY()

public:

	AHASEnemy();

protected:

	virtual void BeginPlay() override;

};
