#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/HASEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Interfaces/HASCombatInterface.h"

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
	
	// 공격당했다면 Chase.
	if (UHASAttributeSet* AS = Cast<UHASAttributeSet>(Enemy->GetAttributeSet()))
	{
		AS->OnAttackedDelegate.AddLambda(
			[AS, BBComp, this, Enemy](UObject* DamageCauser)
			{
				BBComp->SetValueAsBool(FName("IsHit"), true);
				BBComp->SetValueAsObject(FName("ChaseTarget"), DamageCauser);

				FTimerHandle HitHandle;
				// 3초 뒤 어그로 해제.
				GetWorld()->GetTimerManager().SetTimer(HitHandle, [BBComp]
					{
						BBComp->SetValueAsBool(FName("IsHit"), false);
						BBComp->SetValueAsObject(FName("ChaseTarget"), nullptr);
					},
					3.f,
					false);
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.f;
			}
		);
	}

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(FName("TargetActor")));
	if (Enemy && IsValid(TargetActor) && TargetActor->Implements<UHASCombatInterface>())
	{
		const bool IsTargetDead = IHASCombatInterface::Execute_IsDead(TargetActor);
		if (IsTargetDead) BBComp->SetValueAsBool(FName("IsTargetDead"), IsTargetDead);

		// 범위 안에 있다면 Chase.
		if (MinDistance <= 500.f) Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.f;
		else // 범위 밖
		{
			// 범위 밖에서 공격 당했다면 Chase
			if (BBComp->GetValueAsBool(FName("IsHit")))
			{
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.f;
			}
			else
			{
				// 공격당하지 않았다면 Patrol.
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 100.f;
			}
		}
	}

	BBComp->SetValueAsFloat(FName("DistanceToTarget"), MinDistance);
	BBComp->SetValueAsObject(FName("TargetActor"), NearestPlayer);
}
