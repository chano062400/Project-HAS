#include "AbilitySystem/Ability/HASAbility_RushAttack.h"
#include "Interfaces/HASCombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UHASAbility_RushAttack::Rush()
{
	if (GetAvatarActorFromActorInfo()->Implements<UHASCombatInterface>())
	{
		if (IHASCombatInterface* Interface = Cast<IHASCombatInterface>(GetAvatarActorFromActorInfo()))
		{
			AActor* TargetActor = Interface->Execute_GetCombatTarget(GetAvatarActorFromActorInfo());
			FVector TargetLocation = TargetActor->GetActorLocation();
			FVector ToEnemy = TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation();

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = FName("OnMoveCompleted");
			LatentInfo.Linkage = 0;                            
			LatentInfo.UUID = __LINE__;

			UKismetSystemLibrary::MoveComponentTo(
				GetAvatarActorFromActorInfo()->GetRootComponent(),
				TargetLocation,
				ToEnemy.Rotation(),
				false,
				false,
				TargetReachTime,
				false,
				EMoveComponentAction::Move,
				LatentInfo
			);
		}
	}
}

void UHASAbility_RushAttack::OnMoveCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}
