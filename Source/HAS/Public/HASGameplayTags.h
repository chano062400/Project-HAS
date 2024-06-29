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
	FGameplayTag WeaponSocket_Bow;

	/* Ability Tag */

	FGameplayTag Ability_Fire_FireBall;
	FGameplayTag Ability_Fire_FireWall;
	FGameplayTag Ability_Fire_FireMeteor;
	FGameplayTag Ability_HitReact;
	FGameplayTag Ability_MeleeAttack;
	FGameplayTag Ability_RangedAttack;
	FGameplayTag Ability_Dodge;
	FGameplayTag Ability_XP;
	
	/* Montage Tag */

	FGameplayTag Montage_FireBall;
	FGameplayTag Montage_FireWall;
	FGameplayTag Montage_FireMeteor;
	FGameplayTag Montage_HitReact;
	FGameplayTag Montage_MeleeAttack;
	FGameplayTag Montage_RangedAttack;
	FGameplayTag Montage_Dodge;

	/* Effect Tag */

	FGameplayTag Effect_HitReact;

	/* Damage Tag */

	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	/* GameplayCue Tag */
	
	FGameplayTag GameplayCue_HitReact;
	FGameplayTag GameplayCue_FireWall;
	FGameplayTag GameplayCue_Impact_FireBall;;
	FGameplayTag GameplayCue_Impact_FireMeteor;
	FGameplayTag GameplayCue_Impact_ElectricArrow;

	TArray<FGameplayTag> DamageTypes;

private:

	static FHASGameplayTags GameplayTags;
};