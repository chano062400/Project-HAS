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

	/* Input Tag */

	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_SpaceBar;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;

	/* WeaponSocket Tag */

	FGameplayTag WeaponSocket_Staff;

	/* Ability Tag */

	FGameplayTag Ability_Fire_FireBall;
	FGameplayTag Ability_HitReact;
	FGameplayTag Ability_MeleeAttack;
	
	/* Montage Tag */

	FGameplayTag Montage_HitReact;
	FGameplayTag Montage_MeleeAttack;

	/* Effect Tag */

	FGameplayTag Effect_HitReact;

private:

	static FHASGameplayTags GameplayTags;
};