#include "AI/BTTask_Patrol.h"
#include "NavigationSystem.h"
#include "AI/HASAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	FNavLocation NextPatrolLocation;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	bool bResult = NavigationSystem->GetRandomPointInNavigableRadius(ControlledPawn->GetActorLocation(), 500.f, NextPatrolLocation);
	if (bResult)
	{
		if (AHASAIController* AIController = Cast<AHASAIController>(OwnerComp.GetAIOwner()))
		{
			AIController->GetBlackboardComponent()->SetValueAsVector(FName("PatrolLocation"), NextPatrolLocation.Location);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}
