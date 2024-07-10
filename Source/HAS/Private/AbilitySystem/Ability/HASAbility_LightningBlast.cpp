#include "AbilitySystem/Ability/HASAbility_LightningBlast.h"
#include "HASAbilityTypes.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HASGameplayTags.h"

FString UHASAbility_LightningBlast::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString();
}

void UHASAbility_LightningBlast::LightningBlast()
{
	FHASDamageEffectParams DamageEffectParams = MakeDamageEffectParams(nullptr);

	TArray<AActor*> OverlapActors;
	TArray<AActor*> ActorsToIgnore;
	UHASAbilitySystemBlueprintLibrary::GetActorsWithinRadius(GetAvatarActorFromActorInfo(), OverlapActors, ActorsToIgnore, BlastRadius, GetAvatarActorFromActorInfo()->GetActorLocation());

	if (OverlapActors.Num() > 0)
	{
		for (int i = 0; i < OverlapActors.Num(); i++)
		{
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if(bKnockback)
			{
				FVector ToEnemy = (OverlapActors[i]->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
				DrawDebugLine(GetWorld(), GetAvatarActorFromActorInfo()->GetActorLocation(), OverlapActors[i]->GetActorLocation(), FColor::Red, true, 3.f);
				DamageEffectParams.Knockback = ToEnemy * DamageEffectParams.KnockbackMagnitude;
				DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlapActors[i]);
			}

			UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);
		}
	}
}
