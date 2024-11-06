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
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;

	/* Input Tag */

	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_SpaceBar;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;
	FGameplayTag Input_None;

	/* Status Tag */

	FGameplayTag Status_Locked;
	FGameplayTag Status_UnLocked;
	FGameplayTag Status_UnEquipped;
	FGameplayTag Status_Equipped;

	/* WeaponSocket Tag */

	FGameplayTag WeaponSocket_Staff;
	FGameplayTag WeaponSocket_Bow;

	/* Ability Tag */

	FGameplayTag Ability_Fire_FireBall;
	FGameplayTag Ability_Fire_FireWall;
	FGameplayTag Ability_Fire_FireMeteor;
	FGameplayTag Ability_Lightning_LightningBlast;
	FGameplayTag Ability_Lightning_LightningShield;
	FGameplayTag Ability_Lightning_Teleport;
	FGameplayTag Ability_Ice_FrostNova;
	FGameplayTag Ability_Ice_IceTornado;
	FGameplayTag Ability_Ice_IceBeam;
	FGameplayTag Ability_HitReact;
	FGameplayTag Ability_MeleeAttack;
	FGameplayTag Ability_RangedAttack;
	FGameplayTag Ability_Dodge;
	FGameplayTag Ability_XP;
	FGameplayTag Ability_None;
	
	/* Cooldown Tag */

	FGameplayTag Cooldown_Dash;
	FGameplayTag Cooldown_FireBall;
	FGameplayTag Cooldown_FireWall;
	FGameplayTag Cooldown_FireMeteor;
	FGameplayTag Cooldown_LightningBlast;
	FGameplayTag Cooldown_LightningShield;
	FGameplayTag Cooldown_Teleport;
	FGameplayTag Cooldown_FrostNova;
	FGameplayTag Cooldown_IceTornado;
	FGameplayTag Cooldown_IceBeam;
	FGameplayTag Cooldown_None;
	
	/* Montage Tag */

	FGameplayTag Montage_FireBall;
	FGameplayTag Montage_FireWall;
	FGameplayTag Montage_FireMeteor;
	FGameplayTag Montage_LightningBlast;
	FGameplayTag Montage_LightningShield;
	FGameplayTag Montage_Teleport;
	FGameplayTag Montage_FrostNova;
	FGameplayTag Montage_IceTornado;
	FGameplayTag Montage_IceBeam;
	FGameplayTag Montage_HitReact;
	FGameplayTag Montage_MeleeAttack;
	FGameplayTag Montage_JumpAttack;
	FGameplayTag Montage_RangedAttack;
	FGameplayTag Montage_Dodge;

	/* Effect Tag */

	FGameplayTag Effect_HitReact;
	
	/* Potion Tag */
	FGameplayTag Potion_Elixir;
	FGameplayTag Potion_Critical;

	/* Damage Tag */

	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	/* Debuff Tag */

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	FGameplayTag Debuff_Damage;

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_ElectricShock;
	FGameplayTag Debuff_Freeze;


	/* GameplayCue Tag */
	
	FGameplayTag GameplayCue_HitReact;
	FGameplayTag GameplayCue_BossHitReact;
	FGameplayTag GameplayCue_FireWall;
	FGameplayTag GameplayCue_LightningBlast;
	FGameplayTag GameplayCue_Teleport;
	FGameplayTag GameplayCue_IceBeam;
	FGameplayTag GameplayCue_MagicCircle_FireMeteor;
	FGameplayTag GameplayCue_Impact_FireBall;;
	FGameplayTag GameplayCue_Impact_FireMeteor;
	FGameplayTag GameplayCue_Impact_ElectricArrow;
	

	TArray<FGameplayTag> DamageTypes;
	TMap<FGameplayTag, FGameplayTag> DamageToDebuff;

private:

	static FHASGameplayTags GameplayTags;
};