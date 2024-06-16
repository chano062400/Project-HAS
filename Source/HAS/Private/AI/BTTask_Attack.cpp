#include "AI/BTTask_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASCombatInterface.h"
#include "AIController.h"
#include "Character/HASEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner()->GetPawn()->Implements<UHASCombatInterface>())
	{
		if (AHASEnemy* Enemy = Cast<AHASEnemy>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))))
			{
				IHASCombatInterface::Execute_SetCombatTarget(Enemy, TargetActor);
			}

			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(AbilityTag);
			Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
