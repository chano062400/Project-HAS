#include "AbilitySystem/Ability/HASAbility_Summon.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/HASEnemy.h"
#include "Interfaces/HASCombatInterface.h"

void UHASAbility_Summon::Summon()
{
	float HalfRange = Range / 2.f;
	for (float Angle = -HalfRange; Angle <= HalfRange; Angle += Interval)
	{
		const FVector Direction = GetAvatarActorFromActorInfo()->GetActorForwardVector().RotateAngleAxis(Angle, FVector::UpVector);
		FVector PointToSummon = GetAvatarActorFromActorInfo()->GetActorLocation() + Direction * Length;

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			PointToSummon + FVector(0.f, 0.f, 150.f),
			PointToSummon + FVector(0.f, 0.f, -150.f),
			ECollisionChannel::ECC_Visibility
		);

		if (HitResult.bBlockingHit)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetAvatarActorFromActorInfo(),
				SummonEffect,
				HitResult.ImpactPoint
			);

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, 0.f));
			SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());

			int32 RandIdx = FMath::RandRange(0, SummonActorClasses.Num() - 1);
			AHASEnemy* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AHASEnemy>(
				SummonActorClasses[RandIdx],
				SpawnTransform,
				GetAvatarActorFromActorInfo(),
				nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);

			SpawnedEnemy->SetLevel(FMath::RandRange(5, 10));
			SpawnedEnemy->SpawnDefaultController();
			if (GetAvatarActorFromActorInfo()->Implements<UHASCombatInterface>() && SpawnedEnemy->Implements<UHASCombatInterface>())
			{
				if (IHASCombatInterface* Interface = Cast<IHASCombatInterface>(SpawnedEnemy))
				{
					if (IHASCombatInterface* OwnerInterface = Cast<IHASCombatInterface>(GetAvatarActorFromActorInfo()))
					{
						Interface->Execute_SetCombatTarget(SpawnedEnemy, OwnerInterface->Execute_GetCombatTarget(GetAvatarActorFromActorInfo()));
					}
				}
			}
			SpawnedEnemy->FinishSpawning(SpawnTransform);
		}
	}
	
}
