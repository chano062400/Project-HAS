#include "HASGameplayTags.h"
#include "GameplayTagsManager.h"

FHASGameplayTags FHASGameplayTags::GameplayTags;

void FHASGameplayTags::InitializeNativeGameplayTags()
{
	/* Vital Attributes */

	GameplayTags.Attribute_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Vital.Health"),
		FString("Health Attribute")
	);

	GameplayTags.Attribute_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Vital.Mana"),
		FString("Mana Attribute")
	);

	/* Primary Attributes */

	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Vigor"),
		FString("Increases Max Health")
	);

	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"),
		FString("Increases Magical Damages And Max Mana")
	);
	
	GameplayTags.Attribute_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Dexterity"),
		FString("Increases Critical Chance")
	);

	/* Secondary Attribute */

	GameplayTags.Attribute_Secondary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalChance"),
		FString("Chance to Increase Damage")
	);
	
	GameplayTags.Attribute_Secondary_CriticalResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalResistance"),
		FString("Reduces Critical Hit Chance")
	);

	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"),
		FString("MaxHealth Attribute")
	);

	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"),
		FString("Max Mana Attribute")
	);
	
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"),
		FString("Health Regeneration Attribute")
	);
	
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"),
		FString("Mana Regeneration Attribute")
	);

	/* Input Tag */

	GameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"),
		FString("LMB Input Tag")
	);
	
	GameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"),
		FString("RMB Input Tag")
	);
		
	GameplayTags.Input_SpaceBar = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.SpaceBar"),
		FString("SpaceBar Input Tag")
	);
	
	GameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"),
		FString("1 Input Tag")
	);
	
	GameplayTags.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.2"),
		FString("2 Input Tag")
	);
	
	GameplayTags.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.3"),
		FString("3 Input Tag")
	);
	
	GameplayTags.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.4"),
		FString("4 Input Tag")
	);
		
	GameplayTags.Input_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.None"),
		FString("None Input Tag")
	);
	
	/* Status Tag */

	GameplayTags.Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Locked"),
		FString("Ability Locked Status Tag")
	);
	
	GameplayTags.Status_UnLocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.UnLocked"),
		FString("Ability UnLocked Status Tag")
	);
	
	GameplayTags.Status_UnEquipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.UnEquipped"),
		FString("Ability UnEquipped Status Tag")
	);
	
	GameplayTags.Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Equipped"),
		FString("Ability Equipped Status Tag")
	);

	/* WeaponSocket Tag */

	GameplayTags.WeaponSocket_Staff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("WeaponSocket.Staff"),
		FString("Staff Socket Tag")
	);
	
	GameplayTags.WeaponSocket_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("WeaponSocket.Bow"),
		FString("Bow Socket Tag")
	);
		
	GameplayTags.WeaponSocket_TwoHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("WeaponSocket.TwoHand"),
		FString("TwoHand Socket Tag")
	);

	/* Ability Tag */

	GameplayTags.Ability_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Fire.FireBall"),
		FString("FireBall Ability Tag")
	);
	
	GameplayTags.Ability_Fire_FireWall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Fire.FireWall"),
		FString("FireWall Ability Tag")
	);
		
	GameplayTags.Ability_Fire_FireMeteor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Fire.FireMeteor"),
		FString("FireMeteor Ability Tag")
	);
			
	GameplayTags.Ability_Lightning_LightningBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Lightning.LightningBlast"),
		FString("LightningBlast Ability Tag")
	);
			
	GameplayTags.Ability_Lightning_LightningShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Lightning.LightningShield"),
		FString("LightningShield Ability Tag")
	);
				
	GameplayTags.Ability_Lightning_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Lightning.Teleport"),
		FString("Teleport Ability Tag")
	);
			
	GameplayTags.Ability_Ice_FrostNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Ice.FrostNova"),
		FString("FrostNova Ability Tag")
	);
			
	GameplayTags.Ability_Ice_IceTornado = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Ice.IceTornado"),
		FString("IceTornado Ability Tag")
	);
			
	GameplayTags.Ability_Ice_IceBeam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Ice.IceBeam"),
		FString("IceBeam Ability Tag")
	);
	
	GameplayTags.Ability_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.HitReact"),
		FString("HitReact Ability Tag")
	);
		
	GameplayTags.Ability_MeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.MeleeAttack"),
		FString("MeleeAttack Ability Tag")
	);
		
	GameplayTags.Ability_RangedAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.RangedAttack"),
		FString("RangedAttack Ability Tag")
	);
					
	GameplayTags.Ability_ThrowRock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.ThrowRock"),
		FString("ThrowRock Ability Tag")
	);
					
	GameplayTags.Ability_Rush = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Rush"),
		FString("Rush Ability Tag")
	);
						
	GameplayTags.Ability_ChargeScream = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.ChargeScream"),
		FString("ChargeScream Ability Tag")
	);
						
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Summon"),
		FString("Summon Ability Tag")
	);
					
	GameplayTags.Ability_RangedAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.RangedAttack"),
		FString("RangedAttack Ability Tag")
	);
			
	GameplayTags.Ability_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Dodge"),
		FString("Dodge Ability Tag")
	);
				
	GameplayTags.Ability_XP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.XP"),
		FString("XP Passive Ability Tag")
	);
				
	GameplayTags.Ability_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.None"),
		FString("None Ability Tag")
	);
	
	/* Cooldown Tag */

	GameplayTags.Cooldown_Dash= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Dash"),
		FString("Dash Cooldown Tag")
	);
	
	GameplayTags.Cooldown_FireBall= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireBall"),
		FString("FireBall Cooldown Tag")
	);
	
	GameplayTags.Cooldown_FireWall= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireWall"),
		FString("FireWall Cooldown Tag")
	);
	
	GameplayTags.Cooldown_FireMeteor= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireMeteor"),
		FString("FireMeteor Cooldown Tag")
	);
			
	GameplayTags.Cooldown_LightningBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.LightningBlast"),
		FString("LightningBlast Cooldown Tag")
	);
			
	GameplayTags.Cooldown_LightningShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.LightningShield"),
		FString("LightningShield Cooldown Tag")
	);
			
	GameplayTags.Cooldown_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Teleport"),
		FString("Teleport Cooldown Tag")
	);
			
	GameplayTags.Cooldown_FrostNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FrostNova"),
		FString("FrostNova Cooldown Tag")
	);
				
	GameplayTags.Cooldown_IceTornado = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.IceTornado"),
		FString("IceTornado Cooldown Tag")
	);
					
	GameplayTags.Cooldown_IceBeam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.IceBeam"),
		FString("IceBeam Cooldown Tag")
	);
			
	GameplayTags.Cooldown_ThrowRock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.ThrowRock"),
		FString("ThrowRock Cooldown Tag")
	);
		
	GameplayTags.Cooldown_None= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.None"),
		FString("None Cooldown Tag")
	);

	/* Montage Tag */

	GameplayTags.Montage_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.FireBall"),
		FString("FireBall Montage Tag")
	);
	
	GameplayTags.Montage_FireWall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.FireWall"),
		FString("FireWall Montage Tag")
	);
		
	GameplayTags.Montage_FireMeteor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.FireMeteor"),
		FString("FireMeteor Montage Tag")
	);
		
	GameplayTags.Montage_LightningBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.LightningBlast"),
		FString("LightningBlast Montage Tag")
	);
		
	GameplayTags.Montage_LightningShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.LightningShield"),
		FString("LightningShield Montage Tag")
	);
		
	GameplayTags.Montage_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Teleport"),
		FString("Teleport Montage Tag")
	);
		
	GameplayTags.Montage_FrostNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.FrostNova"),
		FString("FrostNova Montage Tag")
	);
		
	GameplayTags.Montage_IceTornado = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.IceTornado"),
		FString("IceTornado Montage Tag")
	);
		
	GameplayTags.Montage_IceBeam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.IceBeam"),
		FString("IceBeam Montage Tag")
	);
	
	GameplayTags.Montage_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.HitReact"),
		FString("HitReact Montage Tag")
	);
	
	GameplayTags.Montage_MeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.MeleeAttack"),
		FString("MeleeAttack Montage Tag")
	);
	
	GameplayTags.Montage_JumpStart = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.JumpStart"),
		FString("JumpStart Montage Tag")
	);
	
	GameplayTags.Montage_JumpEnd = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.JumpEnd"),
		FString("JumpEnd Montage Tag")
	);
	
	GameplayTags.Montage_LiftRock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.LiftRock"),
		FString("LiftRock Montage Tag")
	);
	
	GameplayTags.Montage_ThrowRock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.ThrowRock"),
		FString("ThrowRock Montage Tag")
	);
	
	GameplayTags.Montage_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Summon"),
		FString("Summon Montage Tag")
	);
	
	GameplayTags.Montage_Scream = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Scream"),
		FString("Scream Montage Tag")
	);
	
	GameplayTags.Montage_Rush = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Rush"),
		FString("Rush Montage Tag")
	);
	
	GameplayTags.Montage_RangedAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.RangedAttack"),
		FString("RangedAttack Montage Tag")
	);
		
	GameplayTags.Montage_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Dodge"),
		FString("Dodge Montage Tag")
	);
	
	/* Effect Tag */

	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("HitReact Effect Tag")
	);
	
	/* Potion Tag */

	GameplayTags.Potion_Elixir = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Potion.Elixir"),
		FString("Elixir Potion  Tag")
	);
	
	GameplayTags.Potion_Critical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Potion.Critical"),
		FString("Critical Potion Tag")
	);

	/* Damage Tag */

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Tag")
	);
	
	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Ice"),
		FString("Ice Damage Tag")
	);
	
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Tag")
	);
	
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Tag")
	);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Tag")
	);

	/* Debuff Tag */

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance Tag")
	);
	
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration Tag")
	);
	
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency Tag")
	);
	
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage Tag")
	);
	
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Fire Debuff Tag")
	);
		
	GameplayTags.Debuff_ElectricShock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.ElectricShock"),
		FString("Lightning Debuff Tag")
	);
		
	GameplayTags.Debuff_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Freeze"),
		FString("Freeze Debuff Tag")
	);

	/* GameplayCue Tag */

	GameplayTags.GameplayCue_BossHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.BossHitReact"),
		FString("BossHitReact GameplayCue Tag")
	);
	
	GameplayTags.GameplayCue_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.HitReact"),
		FString("HitReact GameplayCue Tag")
	);
	
	GameplayTags.GameplayCue_FireWall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.FireWall"),
		FString("FireWall GameplayCue Tag")
	);
		
	GameplayTags.GameplayCue_LightningBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.LightningBlast "),
		FString("LightningBlast GameplayCue Tag")
	);
			
	GameplayTags.GameplayCue_IceBeam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.IceBeam "),
		FString("IceBeam GameplayCue Tag")
	);
		
	GameplayTags.GameplayCue_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Teleport "),
		FString("Teleport GameplayCue Tag")
	);
	
	GameplayTags.GameplayCue_MagicCircle_FireMeteor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.MagicCircle_FireMeteor"),
		FString("FireMeteor MagicCircle GameplayCue Tag")
	);

	GameplayTags.GameplayCue_Impact_Rock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Impact.Rock"),
		FString("Rock Impact GameplayCue Tag")
	);

	GameplayTags.GameplayCue_Impact_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Impact.FireBall"),
		FString("FireBall Impact GameplayCue Tag")
	);
	
	GameplayTags.GameplayCue_Impact_FireMeteor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Impact.FireMeteor"),
		FString("FireMeteor Impact GameplayCue Tag")
	);
	
	GameplayTags.GameplayCue_Impact_ElectricArrow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Impact.ElectricArrow"),
		FString("ElectricArrow Impact GameplayCue Tag")
	);

	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Ice);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Lightning);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Physical);

	GameplayTags.DamageToDebuff.Add({ GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn });
	GameplayTags.DamageToDebuff.Add({ GameplayTags.Damage_Lightning, GameplayTags.Debuff_ElectricShock });
	GameplayTags.DamageToDebuff.Add({ GameplayTags.Damage_Ice, GameplayTags.Debuff_Freeze });

}
