#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HASGameModeBase.generated.h"

class UClassInfoDataAsset;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class HAS_API AHASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UClassInfoDataAsset> ClassInformation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityInfo> AbilityInformation;

};
