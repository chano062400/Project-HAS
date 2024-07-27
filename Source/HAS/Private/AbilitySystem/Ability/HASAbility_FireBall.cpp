#include "AbilitySystem/Ability/HASAbility_FireBall.h"
#include "Interfaces/HASCombatInterface.h"
#include "Actor/HASProjectile.h"
#include "AbilitySystemComponent.h"

FString UHASAbility_FireBall::GetAbilityDescription(int32 InAbilityLevel)
{
	float ManaCost = GetManaCost(InAbilityLevel);
	float Cooldown = GetCooldown(InAbilityLevel);

	return FString::Printf(TEXT(
		"<Title> Fire Ball </> \n\n"
		
		"<Level> Rank : %d / 5 </> \n\n"
		
		"Launched a bolt of fire, exploding on impact and dealing <Damage>%.2f</> Fire Damage with <Debuff>%.2f</> chance to apply debuff. \n\n"
	
		"Cost : <Cost>%.2f</> \n"

		"Cooldown : <Cooldown>%.2f</> \n"
	), 
		InAbilityLevel, 
		Damage.GetValueAtLevel(InAbilityLevel),
		DebuffChance,
		-ManaCost,
		Cooldown
	);
}
