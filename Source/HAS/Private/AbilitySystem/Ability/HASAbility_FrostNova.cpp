#include "AbilitySystem/Ability/HASAbility_FrostNova.h"

FString UHASAbility_FrostNova::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString::Printf(TEXT(
		"<Title>Frost Nova</> \n\n <Level>Rank : %d / 5</> \n\n Sends a torrent of frost into the ground, permanently freezing nearby enemies."),
		InAbilityLevel);
}
