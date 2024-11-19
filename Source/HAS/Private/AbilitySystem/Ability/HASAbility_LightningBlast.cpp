#include "AbilitySystem/Ability/HASAbility_LightningBlast.h"
#include "HASAbilityTypes.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HASGameplayTags.h"
#include "GameFramework/Character.h"
#include "Interfaces/HASEnemyInterface.h"
#include "Player/HASPlayerController.h"

FString UHASAbility_LightningBlast::GetAbilityDescription(int32 InAbilityLevel)
{
	float ManaCost = GetManaCost(InAbilityLevel);
	float Cooldown = GetCooldown(InAbilityLevel);

	return FString::Printf(TEXT(
	"<Title> Lightning Blast </> \n\n"
		
	"<Level> Rank : %d / 5 </> \n\n"
		
	"Releases a huge bolt of lightning, dealing <Damage>%.2f</> damage to nearby enemies and pushing them away \n\n" 
	
	" Cost : <Cost>%.2f</> \n"

	" Cooldown : <Cooldown>%.2f</>"
	),
	InAbilityLevel, 
	Damage.GetValueAtLevel(InAbilityLevel),
	-ManaCost,
	Cooldown
	);
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
			if (UHASAbilitySystemBlueprintLibrary::IsFriend(GetAvatarActorFromActorInfo(), OverlapActors[i])) continue;

			if (IHASEnemyInterface* Interface = Cast<IHASEnemyInterface>(OverlapActors[i]))
			{
				ECharacterClass EnemyClass = Interface->GetCharacterClass();
				if (EnemyClass == ECharacterClass::ECC_Boss)
				{
					if (AHASPlayerController* PC = Cast<AHASPlayerController>(CurrentActorInfo->PlayerController))
					{
						PC->ClientShowFloatingImmnueText(OverlapActors[i]);
					}
				}
				else
				{

					FHASDamageEffectParams DamageEffectParams = MakeDamageEffectParams(OverlapActors[i]);

					const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
					if (bKnockback)
					{
						FVector EnemyLoc = OverlapActors[i]->GetActorLocation();
						FVector ToEnemy = (OverlapActors[i]->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
						ToEnemy.Z = 0.f;
						FVector NewEnemyLoc = EnemyLoc + ToEnemy * KnockbackForceMagnitude;

						OverlapActors[i]->SetActorLocation(NewEnemyLoc);
					}
				}
				ApplyDamage(OverlapActors[i], GetAbilityLevel());
			}
		}
	}
}
