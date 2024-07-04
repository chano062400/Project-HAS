#include "AbilitySystem/Ability/HASAbility_FireWall.h"

FString UHASAbility_FireWall::GetAbilityDescription()
{
	return FString::Printf(TEXT(
		"<Title> FireWall </> \n\n <Level> Rank : %d / 5 </> \n\n Summons walls of fire around the player.\n When it overlaps a wall, it deals <Damage>%.2f</> damage and applies a debuff with a <Debuff>%.2f</> chance."),
		GetAbilityLevel(), 45.f, DebuffChance
	);
}