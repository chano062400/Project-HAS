#include "AbilitySystem/Data/AbilityInfo.h"

FHASAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag)
{
	for (FHASAbilityInfo Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	return FHASAbilityInfo();
}
