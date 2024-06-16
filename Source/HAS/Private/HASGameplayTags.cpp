#include "HASGameplayTags.h"
#include "GameplayTagsManager.h"

FHASGameplayTags FHASGameplayTags::GameplayTags;

void FHASGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Ice);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Lightning);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Physical);

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
	
	GameplayTags.Attribute_Secondary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalChance"),
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
	
	/* WeaponSocket Tag */

	GameplayTags.WeaponSocket_Staff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("WeaponSocket.Starff"),
		FString("Staff Socket Tag")
	);

	/* Ability Tag */

	GameplayTags.Ability_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Fire.FireBall"),
		FString("FireBall Ability Tag")
	);
	
	GameplayTags.Ability_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.HitReact"),
		FString("HitReact Ability Tag")
	);
		
	GameplayTags.Ability_MeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.MeleeAttack"),
		FString("MeleeAttack Ability Tag")
	);

	/* Montage Tag */

	GameplayTags.Montage_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.HitReact"),
		FString("HitReact Montage Tag")
	);
	
	GameplayTags.Montage_MeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.MeleeAttack"),
		FString("MeleeAttack Montage Tag")
	);
	
	/* Effect Tag */

	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("HitReact Effect Tag")
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

}
