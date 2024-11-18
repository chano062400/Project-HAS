#include "AbilitySystem/Ability/HASAbility_ChargeScream.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "HASAbilityTypes.h"

void UHASAbility_ChargeScream::KnockbackEnemiesWithinRadius()
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

			UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);
		}
	}
}