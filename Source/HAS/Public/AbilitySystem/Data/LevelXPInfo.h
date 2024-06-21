#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelXPInfo.generated.h"

USTRUCT(BlueprintType)
struct FHASLevelXPInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 Level;

	UPROPERTY(EditDefaultsOnly)
	int32 RequirementXP;

};

/**
 * 
 */
UCLASS()
class HAS_API ULevelXPInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FHASLevelXPInfo> LevelXPInformations;
	
	int32 FindLevelByXP(int32 XP);

};
