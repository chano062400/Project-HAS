#include "AbilitySystem/Data/ClassInfoDataAsset.h"

FClassDefaultInfo UClassInfoDataAsset::GetCharacterClassInfo(ECharacterClass Class)
{
	return CharacterClassInformations.FindChecked(Class);
}
