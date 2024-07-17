#include "AbilitySystem/Ability/HASAbility_IceBeam.h"
#include "HASAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

FString UHASAbility_IceBeam::GetAbilityDescription(int32 InAbilityLevel)
{
	return FString::Printf(TEXT(
		"<Title> Ice Beam </> \n\n <Level> Rank : %d / 5 </> \n\n While pressed, releases an IceBeam in the direction of the player's mouse, dealing <Damage>%.2f</> damage and applying a debuff with a <Debuff>%.2f</> chance"),
		InAbilityLevel, Damage.GetValueAtLevel(InAbilityLevel), DebuffChance
	);
}

void UHASAbility_IceBeam::IceBeamTrace(float AttackRadius, float AttackRange)
{
	TArray<FHitResult> HitResults;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(GetAvatarActorFromActorInfo());

	ETraceTypeQuery TypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);

	const bool bResult = UKismetSystemLibrary::CapsuleTraceMulti(
		GetAvatarActorFromActorInfo(),
		GetAvatarActorFromActorInfo()->GetActorLocation(),
		GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector() * AttackRange,
		AttackRadius,
		AttackRange * 0.5f + AttackRadius,
		TypeQuery,
		false,
		ActorsToIgnore,
		bDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true
	);

	FHASDamageEffectParams DamageEffectParams = MakeDamageEffectParams(nullptr);

	if (HitResults.Num() > 0)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.bBlockingHit)
			{
				DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

				UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);
			}
		}
	}
}
