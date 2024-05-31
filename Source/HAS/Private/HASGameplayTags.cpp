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

}
