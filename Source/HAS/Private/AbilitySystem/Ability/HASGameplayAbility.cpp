#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "AbilitySystem/HASAttributeSet.h"

FString UHASGameplayAbility::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString();
}

float UHASGameplayAbility::GetManaCost(float InAbilityLevel)
{
	float ManaCost = 0.f;

	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo Info : CostEffect->Modifiers)
		{
			if (Info.Attribute == UHASAttributeSet::GetManaAttribute())
			{
				Info.ModifierMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UHASGameplayAbility::GetCooldown(float InAbilityLevel)
{
	float Cooldown = 0.f;

	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, Cooldown);
	}

	return Cooldown;
}
