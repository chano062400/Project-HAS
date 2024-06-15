#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/HASEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_FindNearestPlayer::UBTService_FindNearestPlayer()
{

}

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlledPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	AHASEnemy* Enemy = Cast<AHASEnemy>(ControlledPawn);

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
	
	if (Enemy && IsValid(BBComp->GetValueAsObject(FName("TargetActor"))))
	{
		// 범위 안에 있다면 추격.
		if (MinDistance <= 500.f) Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.f;
		else
		{
			// 범위 밖에서 공격당했다면 추격.
			if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("IsHit")))
			{
				FTimerHandle HitHandle;
				// 3초 뒤 어그로 해제.
				GetWorld()->GetTimerManager().SetTimer(HitHandle, [BBComp]
					{
						BBComp->SetValueAsBool(FName("IsHit"), false);
					},
				3.f, 
				false);
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.f;
			}
			// 공격당하지 않고, 범위 밖에 있다면 Patrol.
			else
			{
				
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 100.f;
			}
		}
	}
	BBComp->SetValueAsFloat(FName("DistanceToTarget"), MinDistance);
	BBComp->SetValueAsObject(FName("TargetActor"), NearestPlayer);
}
