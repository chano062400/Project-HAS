#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "HASEnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHASEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HAS_API IHASEnemyInterface
{
	GENERATED_BODY()

public:

	virtual ECharacterClass GetCharacterClass() = 0;
};
