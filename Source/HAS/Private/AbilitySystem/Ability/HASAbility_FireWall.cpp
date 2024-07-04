#include "AbilitySystem/Ability/HASAbility_FireWall.h"

FString UHASAbility_FireWall::GetAbilityDescription()
{
	return FString::Printf(TEXT(
		"<Title> FireWall </> \n <Level> %d / 5 </> \n\n Summons walls of fire around the player. When it overlaps a wall, it deals <Damage>%f</> damage and applies a debuff with a <Debuff>%f</> chance."),
		GetAbilityLevel(), 45.f, DebuffChance
	);
}
