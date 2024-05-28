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

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere)
	int32 Level = 1;

	virtual int32 GetLevel_Implementation() override { return Level; }

protected:

	virtual void BeginPlay() override;

};
