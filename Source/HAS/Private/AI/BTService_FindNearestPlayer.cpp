#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/HASEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Character/HASEnemy.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (APawn* ControlledPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
		{
			if (AHASEnemy* Enemy = Cast<AHASEnemy>(ControlledPawn))
			{
				TArray<AActor*> Players;
				UGameplayStatics::GetAllActorsWithTag(ControlledPawn, FName("Player"), Players);

				float MinDistance = TNumericLimits<float>::Max();
				AActor* NearestPlayer = nullptr;

				for (AActor* Player : Players)
				{
					if (Player->Implements<UHASCombatInterface>())
					{
						// ���� Player�� ����.
						if (IHASCombatInterface::Execute_IsDead(Player)) continue;

						float DistanceToPlayer = ControlledPawn->GetDistanceTo(Player);
						if (DistanceToPlayer < MinDistance)
						{
							NearestPlayer = Player;
							MinDistance = DistanceToPlayer;
						}
					}
				}

				// ���ݴ��ߴٸ� Chase.
				if (UHASAttributeSet* AS = Cast<UHASAttributeSet>(Enemy->GetAttributeSet()))
				{
					AS->OnAttackedDelegate.AddLambda(
						[AS, BBComp, this, Enemy](UObject* DamageCauser)
						{
							BBComp->SetValueAsBool(FName("IsHit"), true);
							BBComp->SetValueAsObject(FName("ChaseTarget"), DamageCauser);

							FTimerHandle HitHandle;
							// ��׷� ����.
							GetWorld()->GetTimerManager().SetTimer(HitHandle, [BBComp]
								{
									BBComp->SetValueAsBool(FName("IsHit"), false);
									BBComp->SetValueAsObject(FName("ChaseTarget"), nullptr);
								},
								AggroTime,
								false);
							Enemy->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
						}
					);
				}

				AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(FName("TargetActor")));
				if (Enemy && IsValid(TargetActor) && TargetActor->Implements<UHASCombatInterface>())
				{
					const bool IsTargetDead = IHASCombatInterface::Execute_IsDead(TargetActor);
					if (IsTargetDead)
					{
						BBComp->SetValueAsBool(FName("IsTargetDead"), IsTargetDead);
						// �ٸ� Target�� �����ϵ��� ����.
						FTimerHandle TargetDeadHandle;
						GetWorld()->GetTimerManager().SetTimer(TargetDeadHandle, [BBComp]
							{
								BBComp->SetValueAsBool(FName("IsTargetDead"), false);
							},
							1.f,
							false);
					}

					// ���� �ȿ� �ִٸ� Chase.
					if (MinDistance <= CriteriaDIstance) Enemy->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
					else // ���� ��
					{
						// ���� �ۿ��� ���� ���ߴٸ� Chase
						if (BBComp->GetValueAsBool(FName("IsHit")))
						{
							Enemy->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
						}
						else
						{
							// ���ݴ����� �ʾҴٸ� Patrol.
							Enemy->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
						}
					}
				}

				float HealthRatio = 100.f;
				if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(Enemy->GetAttributeSet()))
				{
					float MaxHealth = HASAS->GetMaxHealth();
					float Health = HASAS->GetHealth();
					HealthRatio = (Health / MaxHealth) * 100.f;
				}

				BBComp->SetValueAsFloat(FName("DistanceToTarget"), MinDistance);
				BBComp->SetValueAsObject(FName("TargetActor"), NearestPlayer);
				BBComp->SetValueAsFloat(FName("HealthRatio"), HealthRatio);
			}
		}
	}
}
