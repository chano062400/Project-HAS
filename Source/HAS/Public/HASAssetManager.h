#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HASAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	virtual void StartInitialLoading() override;

};
