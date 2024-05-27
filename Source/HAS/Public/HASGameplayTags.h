#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FHASGameplayTags
{
public:

	static const FHASGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

	/* Vital Attributes */

	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_MaxHealth;
	FGameplayTag Attribute_Vital_Mana;
	FGameplayTag Attribute_Vital_MaxMana;

	/* Primary Attributes */

	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Dexterity;

	/* Secondary Attributes */

	FGameplayTag Attribute_Secondary_CriticalChance;
	FGameplayTag Attribute_Secondary_CriticalResistance;

private:

	static FHASGameplayTags GameplayTags;
};