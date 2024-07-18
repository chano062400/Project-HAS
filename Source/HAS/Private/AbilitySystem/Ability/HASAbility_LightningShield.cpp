#include "AbilitySystem/Ability/HASAbility_LightningShield.h"

FString UHASAbility_LightningShield::GetAbilityDescription(int32 InAbilityLevel)
{
	float ManaCost = GetManaCost(InAbilityLevel);
	float Cooldown = GetCooldown(InAbilityLevel);

	return FString::Printf(TEXT(
		"<Title>Lightning Shield</> \n\n"
		
		"<Level> Rank : %d / 5</> \n\n"
		
		"Summons a strong magnetic field that surrounds the player, inflicts <Damage>%.2f</> damage to enemies that come into contact with the magnetic field, and applies a debuff with a <Debuff>%.2f</> chance."
		
		"<Cost> Cost : %.2f</> \n"

		"<Cooldown> Cooldown : %.2f</>"
	),
		InAbilityLevel,
		Damage.GetValueAtLevel(InAbilityLevel),
		DebuffChance,
		ManaCost,
		Cooldown
	);
}
