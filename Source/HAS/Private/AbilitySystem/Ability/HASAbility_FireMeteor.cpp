#include "AbilitySystem/Ability/HASAbility_FireMeteor.h"
#include "Actor/HASProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/HASProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UHASAbility_FireMeteor::SpawnMeteor(const FVector& TargetLocation)
{
	// 생성위치
	FVector SpawnLoc = FVector(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + 1000.f);
	/*DrawDebugSphere(GetWorld(), SpawnLoc, 50.f, 12, FColor::Blue, true);*/

	// 떨어질 곳 랜덤위치.
	float RandValueX = FMath::RandRange(-Radius, Radius);
	float RandValueY = FMath::RandRange(-Radius, Radius);
	FVector EndLoc = FVector(TargetLocation.X + RandValueX, TargetLocation.Y + RandValueY, 0.f);
	FRotator Rot = (EndLoc - SpawnLoc).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetRotation(Rot.Quaternion());
	SpawnTransform.SetLocation(SpawnLoc);

	/*DrawDebugLine(GetWorld(), SpawnLoc, EndLoc, FColor::Red, true);
	DrawDebugSphere(GetWorld(), EndLoc, 50.f, 12, FColor::Blue, true);*/

	Projectile = GetWorld()->SpawnActorDeferred<AHASProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParams(nullptr);

	Projectile->FinishSpawning(SpawnTransform);
}

FString UHASAbility_FireMeteor::GetAbilityDescription(int32 InAbilityLevel)
{
	float ManaCost = GetManaCost(InAbilityLevel);
	float Cooldown = GetCooldown(InAbilityLevel);

	return FString::Printf(TEXT(
		"<Title> Fire Meteor </> \n\n"
		
		"<Level> Rank : %d / 5 </> \n\n"
		
		"Meteors fall from the sky, dealing <Damage>%.2f</> Fire damage to enemies within the range based on where the meteor fell and causing a debuff with a <Debuff>%.2f</> chance.</>\n\n"
	
		" Cost : <Cost>%.2f</> \n"

		" Cooldown : <Cooldown>%.2f</>"
	),
		InAbilityLevel,
		Damage.GetValueAtLevel(InAbilityLevel),
		DebuffChance,
		-ManaCost,
		Cooldown
	);
}
