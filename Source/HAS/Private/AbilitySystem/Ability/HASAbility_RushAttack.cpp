#include "AbilitySystem/Ability/HASAbility_RushAttack.h"
#include "Interfaces/HASCombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "HASAbilityTypes.h"
#include "GameplayCueManager.h"
#include "HASGameplayTags.h"
#include "AbilitySystemComponent.h"
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
	TArray<AActor*> OverlapActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(GetAvatarActorFromActorInfo());

	UHASAbilitySystemBlueprintLibrary::GetActorsWithinRadius(GetAvatarActorFromActorInfo(), OverlapActors, ActorsToIgnore, KnockBackRadius, GetAvatarActorFromActorInfo()->GetActorLocation());

	if (OverlapActors.Num() > 0)
	{
		for (int i = 0; i < OverlapActors.Num(); i++)
		{
			if (UHASAbilitySystemBlueprintLibrary::IsFriend(GetAvatarActorFromActorInfo(), OverlapActors[i])) continue;

			FHASDamageEffectParams DamageEffectParams = MakeDamageEffectParams(OverlapActors[i]);

			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);
			}
		}
	}
	FGameplayCueParameters Params;
	Params.SourceObject = GetAvatarActorFromActorInfo();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(FHASGameplayTags::Get().GameplayCue_Impact_Rush, Params);
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}
