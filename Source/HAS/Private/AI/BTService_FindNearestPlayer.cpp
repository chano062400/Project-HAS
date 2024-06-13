#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlledPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsWithTag(ControlledPawn, FName("Player"), Players);

	float MinDistance = TNumericLimits<float>::Max();
	AActor* NearestPlayer = nullptr;

	for (AActor* Player : Players)
	{
		float DistanceToPlayer = ControlledPawn->GetDistanceTo(Player);
		if (DistanceToPlayer < MinDistance)
		{
			NearestPlayer = Player;
			MinDistance = DistanceToPlayer;
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, MinDistance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetActorSelector, NearestPlayer);
}
