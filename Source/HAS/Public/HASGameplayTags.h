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
	FGameplayTag Attribute_Vital_Mana;

	/* Primary Attributes */

	FGameplayTag Attribute_Primary_Vigor;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Dexterity;

	/* Secondary Attributes */

	FGameplayTag Attribute_Secondary_CriticalChance;
	FGameplayTag Attribute_Secondary_CriticalResistance;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;

private:

	static FHASGameplayTags GameplayTags;
};