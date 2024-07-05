#include "AbilitySystem/Ability/HASAbility_FireWall.h"
#include "AbilitySystem/HASAttributeSet.h"

FString UHASAbility_FireWall::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString::Printf(TEXT(
		"<Title> Fire Wall </> \n\n <Level> Rank : %d / 5 </> \n\n Summons walls of fire around the player.\n When it overlaps a wall, it deals <Damage>%.2f</> damage and applies a debuff with a <Debuff>%.2f</> chance.</>"),
		InAbilityLevel, Damage.GetValueAtLevel(InAbilityLevel) , DebuffChance
	);
}
