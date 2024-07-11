#include "AbilitySystem/Ability/HASAbility_LightningBlast.h"
#include "HASAbilityTypes.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HASGameplayTags.h"
#include "GameFramework/Character.h"

FString UHASAbility_LightningBlast::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString();
}

void UHASAbility_LightningBlast::LightningBlast()
{

	TArray<AActor*> OverlapActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(GetAvatarActorFromActorInfo());

	UHASAbilitySystemBlueprintLibrary::GetActorsWithinRadius(GetAvatarActorFromActorInfo(), OverlapActors, ActorsToIgnore, BlastRadius, GetAvatarActorFromActorInfo()->GetActorLocation());

	if (OverlapActors.Num() > 0)
	{
		for (int i = 0; i < OverlapActors.Num(); i++)
		{
			if(UHASAbilitySystemBlueprintLibrary::IsFriend(GetAvatarActorFromActorInfo(), OverlapActors[i])) continue;

			FHASDamageEffectParams DamageEffectParams = MakeDamageEffectParams(OverlapActors[i]);

			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if(bKnockback)
			{
				FVector EnemyLoc = OverlapActors[i]->GetActorLocation();
				FVector ToEnemy = (OverlapActors[i]->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
				ToEnemy.Z = 0.f;
				FVector NewEnemyLoc = EnemyLoc + ToEnemy * KnockbackForce;
				
				OverlapActors[i]->SetActorLocation(NewEnemyLoc);
			}

			UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);
		}
	}
}
