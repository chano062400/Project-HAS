#include "AbilitySystem/Ability/HASAbility_IceTornado.h"

FString UHASAbility_IceTornado::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString::Printf(TEXT(
		"<Title> Ice Tornado </> \n\n <Level> Rank : %d / 5 </> \n\n Summons a tornado that follows the player. Deals <Damage>%.2f</> damage to enemies it touches while moving, and applies a debuff with a <Debuff>%.2f</> chance."
	), InAbilityLevel, Damage.GetValueAtLevel(InAbilityLevel), DebuffChance
		);
}

