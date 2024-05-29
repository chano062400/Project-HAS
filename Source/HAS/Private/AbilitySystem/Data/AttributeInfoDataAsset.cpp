#include "AbilitySystem/Data/AttributeInfoDataAsset.h"
#include "GameplayTagContainer.h"

FHASAttributeInfo UAttributeInfoDataAsset::FindAttributeInfoByTag(const FGameplayTag& AttributeTag)
{
	for (const FHASAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	return FHASAttributeInfo();
}
