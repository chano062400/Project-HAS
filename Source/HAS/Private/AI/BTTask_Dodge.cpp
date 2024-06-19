#include "AI/BTTask_Dodge.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "HASGameplayTags.h"

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComp.GetAIOwner()->GetPawn()))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FHASGameplayTags::Get().Ability_Dodge);
		ASC->TryActivateAbilitiesByTag(TagContainer);
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
