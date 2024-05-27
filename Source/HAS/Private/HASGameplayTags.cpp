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
	
	GameplayTags.Attribute_Vital_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Vital.MaxHealth"),
		FString("MaxHealth Attribute")
	);
	
	GameplayTags.Attribute_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Vital.Mana"),
		FString("Mana Attribute")
	);
	
	GameplayTags.Attribute_Vital_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Vital.MaxMana"),
		FString("Max Mana Attribute")
	);

	/* Primary Attributes */

	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"),
		FString("Increases Magical Damages")
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

}
